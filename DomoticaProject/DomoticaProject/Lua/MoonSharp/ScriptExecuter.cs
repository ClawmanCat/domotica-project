using System;
using System.Collections.Generic;
using System.Text;
using MoonSharp.Interpreter;

namespace DomoticaProject.Lua.MoonSharp {
    static class ScriptExecuter {
        static void Execute(List<string> src) {
            Script script = new Script();
            script.DoString(string.Join("", src));

            script.Call("Main");
        }
    }
}
