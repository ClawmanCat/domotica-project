using DomoticaProject.Utility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using Timer = System.Timers.Timer;

namespace DomoticaProject.Server {
    class ConnectionManager {
        public delegate void OnAnswerReceived(string answer);       // Called when a response is received to a message.
        public delegate void OnAnswerTimeout();                     // Called when no response is received to a message within the timeout period.

        private Thread thread;
        private bool ShouldShutdown;
        private Mutex mtx;

        private Dictionary<int, (OnAnswerReceived, OnAnswerTimeout)> AwaitedAnswers;
        private int NextMsgID;

        private Socket socket;
        private IPAddress address;
        private int port;

        public ConnectionManager(IPAddress address, int port) {
            this.address = address;
            this.port = port;
            this.ShouldShutdown = false;
            this.NextMsgID = 0;

            this.AwaitedAnswers = new Dictionary<int, (OnAnswerReceived, OnAnswerTimeout)>();
            this.mtx = new Mutex();

            this.socket = new Socket(this.address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            this.socket.ReceiveTimeout = 250;

            // Thread listens for responses to sent messages.
            this.thread = new Thread(() => {
                while (!ShouldShutdown) {
                    if (socket.Connected) {
                        mtx.WaitOne();

                        // Release mutex if nothing to read.
                        int available = socket.Available;
                        if (available == 0) {
                            mtx.ReleaseMutex();
                            continue;
                        }

                        // Read response.
                        byte[] response = new byte[available];
                        socket.Receive(response, available, SocketFlags.None);

                        if (available < 5 || response.Last() != '\0') {
                            // Invalid response.
                            mtx.ReleaseMutex();
                            continue;
                        }

                        // Extract message data.
                        byte[] ResponseArray = response.ToArray();
                        int ID     = BitConverter.ToInt32(ResponseArray, 0);
                        string msg = Encoding.ASCII.GetString(ResponseArray.Subset(4, ResponseArray.Count() - 4).ToArray());

                        // Find the associated message and perform callbacks.
                        if (AwaitedAnswers.TryGetValue(ID, out (OnAnswerReceived, OnAnswerTimeout) value)) {
                            value.Item1(msg);
                            AwaitedAnswers.Remove(ID);
                        }

                        mtx.ReleaseMutex();
                    }
                }
            });

            thread.Start();
            socket.BeginConnect(new IPEndPoint(address, port), null, null);
        }

        public void SendMessage(byte device, byte port, string message, OnAnswerReceived OnReceived, OnAnswerTimeout OnTimeout, TimeSpan timeout) {
            new Task(() => {
                mtx.WaitOne();

                int ID = NextMsgID++;

                // Construct message
                byte[] msg = BitConverter.GetBytes(ID)
                    .Concat(new byte[] { device, port })
                    .Concat(Encoding.ASCII.GetBytes(message))
                    .Concat(new byte[] { (byte) '\0' })
                    .ToArray();

                DateTime start = DateTime.Now;

                // Wait for connection if not already connected.
                while (!socket.Connected && DateTime.Now - start < timeout) {}

                if (!socket.Connected) {
                    // No connection in time -> call timeout callback.
                    OnTimeout();
                } else {
                    // Send message & add it to the queue of messages awaiting an answer.
                    socket.Send(msg);
                    AwaitedAnswers.Add(ID, (OnReceived, OnTimeout));

                    // Run a timer to remove it from the queue if it times out.
                    Timer timer = new Timer {
                        AutoReset = false,
                        Interval = (timeout - (DateTime.Now - start)).TotalMilliseconds
                    };

                    timer.Elapsed += (object src, ElapsedEventArgs e) => {
                        mtx.WaitOne();
                        
                        if (AwaitedAnswers.TryGetValue(ID, out (OnAnswerReceived, OnAnswerTimeout) value)) {
                            value.Item2();
                            AwaitedAnswers.Remove(ID);
                        }

                        mtx.ReleaseMutex();
                    };

                    timer.Start();
                }

                mtx.ReleaseMutex();
            }).Start();
        }

        public bool IsConnected() {
            return socket.Connected;
        }

        public void Shutdown() {
            ShouldShutdown = true;
            thread.Join();
            socket.Close();
        }
    }
}
