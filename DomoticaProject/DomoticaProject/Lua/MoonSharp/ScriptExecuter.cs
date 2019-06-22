using System;
using System.Collections.Generic;
using System.Text;
using DomoticaProject.Server;
using MoonSharp.Interpreter;

namespace DomoticaProject.Lua.MoonSharp {
    static class ScriptExecuter {
        public enum ScriptErrorType { SYNTAX_ERROR, RUNTIME_ERROR, INTERNAL_ERROR, CONNECTION_ERROR };
        public delegate void OnError(ScriptErrorType type, string reason);

        public static void Execute(List<string> src, OnError callback) {
            Script script = new Script();
            foreach (Type t in APIRegistry.GetStaticGlobals()) script.Globals[t.Name] = t;

            try {
                script.DoString(string.Join(" ", src));
            } catch (SyntaxErrorException e) {
                callback(ScriptErrorType.SYNTAX_ERROR, e.Message);
            } catch (ScriptRuntimeException e) {
                callback(ScriptErrorType.RUNTIME_ERROR, e.Message);
            } catch (InternalErrorException e) {
                callback(ScriptErrorType.INTERNAL_ERROR, e.Message);
            } catch (ConnectionException e) {
                callback(ScriptErrorType.CONNECTION_ERROR, e.Message);
            }
        }
    }
}
