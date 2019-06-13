using DomoticaProject.Utility;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace DomoticaProject.Server {
    class Command {
        public string name { get; set; }
        public byte id { get; set; }
    }

    class ServerCommandManager {
        private static Command ListCommandsCommand = new Command { name = "LIST_COMMANDS", id = 0x00 };

        public delegate void OnResponseReceived(List<string> response);
        public delegate void OnResponseTimeout();

        private ConnectionManager connection;
        private List<Command> commands;
        private bool AreCommandsOutdated;


        public ServerCommandManager(ConnectionManager connection) {
            this.connection = connection;
            this.commands = new List<Command>();
            this.AreCommandsOutdated = true;
        }

        // Attempts to get a list of enabled commands. If there is no server connection,
        // it returns the last available list of commands.
        public IReadOnlyList<Command> GetEnabledCommands(TimeSpan timeout) {
            if (AreCommandsOutdated) {
                var response = SendCommand(ListCommandsCommand, timeout);
                if (response == null) return commands;

                List<(string, string)> NewCommands = response.SplitPairs(' ');

                commands.Clear();
                foreach (var command in NewCommands) commands.Add(new Command{ name = command.Item2, id = Byte.Parse(command.Item1) });

                AreCommandsOutdated = false;
            }

            return commands;
        }

        // Send a command asynchronously, and perform the appropriate callback when a response is received,
        // or the request times out.
        public void SendCommandAsync(Command command, OnResponseReceived OnReceived, OnResponseTimeout OnTimeout, TimeSpan timeout) {
            connection.SendMessage(
                0, 
                command.id, 
                "",
                (string answer) => {
                    OnReceived(new List<string>(answer.Split('\n')));
                },
                () => {
                    OnTimeout();
                },
                timeout
            );
        }

        // Send a command and block the calling thread until a response is obtained. (or the request times out.)
        // If the request times out, null is returned.
        public List<string> SendCommand(Command command, TimeSpan timeout) {
            ManualResetEvent evnt = new ManualResetEvent(false);
            List<string> result = null;

            connection.SendMessage(
                0,
                command.id,
                "",
                (string response) => {
                    result = new List<string>(response.Split('\n'));
                    evnt.Set();
                },
                () => {
                    evnt.Set();
                },
                timeout
            );

            evnt.WaitOne();
            return result;
        }
    }
}
