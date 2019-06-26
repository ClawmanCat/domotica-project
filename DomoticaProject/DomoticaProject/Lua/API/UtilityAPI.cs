using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace DomoticaProject.Lua.API {
    class UtilityAPI : ILuaAPI {
        public static void Sleep(int ms) {
            Thread.Sleep(ms);
        }
    }
}
