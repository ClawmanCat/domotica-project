using DomoticaProject.Utility;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace DomoticaProject.Server {
    public class Command {
        public string DisplayName { get; set; }
        public string Description { get; set; }
        public byte ID;

        // Print ID formatted as a 3-digit number with a hash symbol preceding it.
        public string FormattedID {
            get {
                string result = ((int) ID).ToString();
                while (result.Length < 3) result = '0' + result;
                result = '#' + result;

                return result;
            }
        }
    }

    public class ServerCommandManager {
        public static readonly Command ListCommandsCommand = new Command { DisplayName = "LIST_COMMANDS", ID = 0x00 };

        public delegate void OnResponseReceived(List<string> response);
        public delegate void OnResponseTimeout();

        public delegate void OnCommandListReceived(IReadOnlyList<Command> commands);

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

                commands = DecodeCommands(response);
                AreCommandsOutdated = false;
            }

            return commands;
        }

        // Get available commands asynchronously, and perform the appropriate callback when a response is received,
        // or the request times out.
        public void GetEnabledCommandsAsync(OnCommandListReceived OnReceived, OnResponseTimeout OnTimeout, TimeSpan timeout) {
            if (AreCommandsOutdated) {
                SendCommandAsync(
                    ListCommandsCommand,
                    (List<string> response) => {
                        commands = DecodeCommands(response);
                        AreCommandsOutdated = false;

                        OnReceived(commands);
                    },
                    OnTimeout,
                    timeout
                );
            } else {
                OnReceived(commands);
            }
        }

        // Send a command asynchronously, and perform the appropriate callback when a response is received,
        // or the request times out.
        public void SendCommandAsync(Command command, OnResponseReceived OnReceived, OnResponseTimeout OnTimeout, TimeSpan timeout) {
            connection.SendMessage(
                0, 
                command.ID, 
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
                command.ID,
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

        // Construct command objects from the Arduino server's response to the GetCommandsCommand
        private static List<Command> DecodeCommands(List<string> data) {
            List<Command> result = new List<Command>();

            List<(string, string)> commands = data.SplitPairs(' ');
            foreach (var command in commands) result.Add(new Command { DisplayName = command.Item2, ID = Byte.Parse(command.Item1) });

            return result;
        }
    }
}
