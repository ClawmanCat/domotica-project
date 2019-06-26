using SQLite;
using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace DomoticaProject.Utility {
    public interface IDBAccess {
        SQLiteConnection Connect();
    }

    public static class DBProvider {
        public static SQLiteConnection Connect() {
            return DependencyService.Get<IDBAccess>().Connect();
        }
    }
}
