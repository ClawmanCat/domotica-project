using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using DomoticaProject.Server;
using MoonSharp.Interpreter;

namespace DomoticaProject.Lua.MoonSharp {
    static class ScriptExecuter {
        public enum ScriptErrorType { SYNTAX_ERROR, RUNTIME_ERROR, INTERNAL_ERROR, CONNECTION_ERROR };
        public delegate void OnError(ScriptErrorType type, string reason);

        public static void Execute(List<string> src, OnError callback) {
            new Task(() => {
                Script script = new Script();
                foreach (Type t in APIRegistry.GetStaticGlobals()) script.Globals[t.Name] = t;
                foreach (var pair in APIRegistry.GetDynMethods()) script.Globals[pair.Item2] = new Func<string, string>(pair.Item1);

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
            }).Start();
        }
    }
}
