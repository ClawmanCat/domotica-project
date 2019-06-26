using DomoticaProject.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DomoticaProject.Lua.API {
    public class ArduinoAPI : ILuaAPI {
        public static List<Command> GetSupportedCommands() {
            if (App.ArduinoConnection == null) throw new ConnectionException("No connection to Arduino has been established.");

            var commands = App.ArduinoConnection.CommandManager.GetEnabledCommands(TimeSpan.FromMilliseconds(2500));
            if (commands == null) throw new ConnectionException("Failed to get command list: request timed out after 2500 ms.");

            List<Command> result = new List<Command>();
            foreach (Command c in commands) result.Add(c);

            return result;
        }


        public static List<string> ExecuteCommand(Command cmd) {
            if (App.ArduinoConnection == null) throw new ConnectionException("No connection to Arduino has been established.");
    
            List<string> response = App.ArduinoConnection.CommandManager.SendCommand(cmd, TimeSpan.FromMilliseconds(2500));
            if (response == null) throw new ConnectionException("Failed to get command list: request timed out after 2500 ms.");

            return response;
        }


        public static List<ArduinoAttachable> GetAttachables() {
            if (App.ArduinoConnection == null) throw new ConnectionException("No connection to Arduino has been established.");

            var response = App.ArduinoConnection.AttachableManager.GetAttachables();
            if (response == null) throw new ConnectionException("Failed to get attachable list: request timed out after 2500 ms.");

            List<ArduinoAttachable> result = new List<ArduinoAttachable>();
            foreach (ArduinoAttachable a in response) result.Add(a);

            return result;
        }


        public static string ExecuteOnAttachable(ArduinoAttachable a, string args) {
            return App.ArduinoConnection.AttachableManager.RunAttachable(a, args);
        }
    }
}
