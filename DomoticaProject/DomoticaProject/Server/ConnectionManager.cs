using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DomoticaProject.Server {
    class ConnectionManager {
        public delegate void OnResponseReceived(byte[] data);

        private const int port = 5300;

        private IPAddress address;
        private IPEndPoint EndPoint;
        private Socket socket;

        private Thread thread;
        private bool SignalEnd;

        private List<(long, OnResponseReceived)> SentMessages;
        private long NextMsgID = 0;

        public ConnectionManager(IPAddress address) {
            this.address = address;
            this.EndPoint = new IPEndPoint(address, port);

            this.socket = new Socket(this.address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            this.socket.Connect(EndPoint);

            this.thread = new Thread(() => {
                while (!SignalEnd) {
                    // Receive message from server.
                    byte[] msg = new byte[1024];
                    int size = socket.Receive(msg);

                    // Message too small? -> just ignore it.
                    if (size < sizeof(long)) continue;

                    // Find the message this message is a response to.
                    long id = BitConverter.ToInt64(msg, 0);
                    int at = SentMessages.FindIndex(0, SentMessages.Count, ((long, OnResponseReceived) elem) => { return elem.Item1 == id; });

                    // Perform the callback and remove the message from the list of messages that still require a response.
                    if (at != -1) {
                        var elem = SentMessages[at];
                        elem.Item2(msg);

                        SentMessages.RemoveAt(at);
                    }
                }
            });
        }


        ~ConnectionManager() {
            SignalEnd = true;
            thread.Join();
        }


        public void SendMessage(byte[] message, OnResponseReceived callback) {
            long MsgID = NextMsgID++;

            // Add ID to message and send it. Store the callback so it can be called if a response is received.
            socket.Send(BitConverter.GetBytes(MsgID).Concat(message).ToArray());
            SentMessages.Add((MsgID, callback));
        }
    }
}
