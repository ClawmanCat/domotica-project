using DomoticaProject.Lua.API;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MoonSharp.Interpreter;
using DomoticaProject.Server;

namespace DomoticaProject.Lua.MoonSharp {
    static class APIRegistry {
        public delegate string DynamicMethod(string data);

        private static List<Type> StaticGlobals = new List<Type>();
        private static List<(DynamicMethod, string)> DynMethods = new List<(DynamicMethod, string)>();

        public static IReadOnlyList<Type> GetStaticGlobals() => StaticGlobals;
        public static IReadOnlyList<(DynamicMethod, string)> GetDynMethods() => DynMethods;

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

        public static void RegisterMethod(DynamicMethod method, string name) {
            DynMethods.Add((method, name));
        }

        public static void RemoveMethod(string name) {
            DynMethods.RemoveAt(
                DynMethods.FindIndex(x => x.Item2 == name)    
            );
        }
    }
}
