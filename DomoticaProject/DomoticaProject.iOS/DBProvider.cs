using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DomoticaProject.Utility;
using Foundation;
using SQLite;
using UIKit;
using Xamarin.Forms;

[assembly: Dependency(typeof(DomoticaProject.iOS.DBProvider))]
namespace DomoticaProject.iOS {
    class DBProvider : IDBAccess {
        public SQLiteConnection Connect() {
            throw new NotImplementedException();
        }
    }
}