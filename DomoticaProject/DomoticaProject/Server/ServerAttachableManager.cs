using DomoticaProject.Utility;
using MoonSharp.Interpreter.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace DomoticaProject.Server {
    public class ArduinoAttachable {
        public byte device { [MoonSharpVisible(true)] get; [MoonSharpVisible(false)] set; }
        public byte port   { [MoonSharpVisible(true)] get; [MoonSharpVisible(false)] set; }
        public string name { [MoonSharpVisible(true)] get; [MoonSharpVisible(false)] set; }
    }

    public class ServerAttachableManager {
        private ConnectionManager connection;

        private List<ArduinoAttachable> attachables;
        private bool IsOutdated;

        public ServerAttachableManager(ConnectionManager connection) {
            this.connection = connection;

            this.attachables = new List<ArduinoAttachable>();
            this.IsOutdated = true;
        }

        public IReadOnlyList<ArduinoAttachable> GetAttachables() {
            if (IsOutdated) {
                Command GetAttachablesCmd = ((IEnumerable<Command>) connection.CommandManager.GetEnabledCommands(TimeSpan.FromMilliseconds(2500)))
                    .Where(x => x.DisplayName == "LIST_ATTACHABLES")
                    .FirstOrDefault();

                attachables = DecodeAttachables(connection.CommandManager.SendCommand(GetAttachablesCmd, TimeSpan.FromMilliseconds(2500)));
                IsOutdated = false;

                return attachables;
            } else {
                return attachables;
            }
        }

        public string RunAttachable(ArduinoAttachable a, string data) {
            if (data == null) data = "";

            string result = "";
            ManualResetEvent evnt = new ManualResetEvent(false);

            connection.SendMessage(
                a.device, 
                a.port, 
                data,
                (string answer) => {
                    result = answer;
                    evnt.Set();
                },
                () => {
                    ToastProvider.ShowToast("A request for attachable " + a.name + " timed out.");
                },
                TimeSpan.FromMilliseconds(2500)
            );

            evnt.WaitOne();
            return result;
        }

        private List<ArduinoAttachable> DecodeAttachables(List<string> attachables) {
            List<ArduinoAttachable> result = new List<ArduinoAttachable>();

            // Message has the form D:P:NNNN...
            // where D is the device, P is the port and NNNN... is the name string.
            foreach (string str in attachables.PopBackEmptyStrings()) {
                result.Add(new ArduinoAttachable {
                    device = (byte) Int32.Parse(str[0].ToString()),
                    port   = (byte) Int32.Parse(str[2].ToString()),
                    name   = str.Substring(4)
                });
            }

            return result;
        }
    }
}
