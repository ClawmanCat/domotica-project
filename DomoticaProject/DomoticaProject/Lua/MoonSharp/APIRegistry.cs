using DomoticaProject.Lua.API;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MoonSharp.Interpreter;
using DomoticaProject.Server;

namespace DomoticaProject.Lua.MoonSharp {
    static class APIRegistry {
        private static List<Type> StaticGlobals = new List<Type>();
        public static IReadOnlyList<Type> GetStaticGlobals() => StaticGlobals;


        public static void RegisterAll() {
            Script.GlobalOptions.RethrowExceptionNested = true;

            UserData.RegisterType(typeof(Command));

            var APIClasses = AppDomain.CurrentDomain.GetAssemblies()
                .SelectMany(x => x.GetTypes())
                .Where(x => typeof(ILuaAPI).IsAssignableFrom(x));

            foreach (var t in APIClasses) {
                UserData.RegisterType(t);
                StaticGlobals.Add(t);
            }
        }
    }
}
