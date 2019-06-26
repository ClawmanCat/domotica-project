using DomoticaProject.MVVM;
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
using Xamarin.Forms;
using Timer = System.Timers.Timer;

namespace DomoticaProject.Server {
    public class ConnectionManager {
        public delegate void OnAnswerReceived(string answer);       // Called when a response is received to a message.
        public delegate void OnAnswerTimeout();                     // Called when no response is received to a message within the timeout period.

        public readonly ServerCommandManager CommandManager;
        public readonly ServerAttachableManager AttachableManager;

        private Thread thread;
        private bool ShouldShutdown;
        private Mutex mtx;

        private Dictionary<int, (OnAnswerReceived, OnAnswerTimeout)> AwaitedAnswers;
        private int NextMsgID;

        private Socket socket;
        private IPAddress address;
        private int port;

        public ConnectionManager(IPAddress address, int port) {
            this.CommandManager = new ServerCommandManager(this);
            this.AttachableManager = new ServerAttachableManager(this);

            this.address = address;
            this.port = port;
            this.ShouldShutdown = false;
            this.NextMsgID = 0;

            this.AwaitedAnswers = new Dictionary<int, (OnAnswerReceived, OnAnswerTimeout)>();
            this.mtx = new Mutex();

            this.socket = new Socket(this.address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

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
                        if (msg.Last() == '\0') msg = msg.Substring(0, msg.Length - 1);

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
            socket.BeginConnect(
                new IPEndPoint(address, port), 
                (IAsyncResult r) => {
                    Device.BeginInvokeOnMainThread(() => {
                        // Load aliases as soon as there's a connection.
                        AttachableAliasingPage.instance.init();
                    });
                },
                null
            );
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

                // Send message & add it to the queue of messages awaiting an answer.
                socket.SendTimeout = (int) timeout.TotalMilliseconds;

                try {
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
                } catch (SocketException) {
                    OnTimeout();
                }

                mtx.ReleaseMutex();
            }).Start();
        }

        public bool IsConnected() {
            // TODO: ping server to check.
            return socket.Connected;
        }

        public (IPAddress, int) GetConnectionAddress() {
            return (address, port);
        }

        public void Shutdown() {
            ShouldShutdown = true;
            thread.Join();
            socket.Close();
        }
    }
}
