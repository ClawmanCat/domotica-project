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
    }
}
