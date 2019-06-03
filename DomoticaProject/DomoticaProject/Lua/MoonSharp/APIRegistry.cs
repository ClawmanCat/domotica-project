using DomoticaProject.Lua.API;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MoonSharp.Interpreter;

namespace DomoticaProject.Lua.MoonSharp {
    static class APIRegistry {
        public static void RegisterAll() {
            Type[] APIClasses = (Type[]) AppDomain.CurrentDomain.GetAssemblies()
                .SelectMany(x => x.GetTypes())
                .Where(x => typeof(ILuaAPI).IsAssignableFrom(x));

            foreach (Type t in APIClasses) UserData.RegisterType(t);
        }
    }
}
