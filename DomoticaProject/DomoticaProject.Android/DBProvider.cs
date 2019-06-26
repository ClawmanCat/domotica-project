using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using DomoticaProject.Utility;
using SQLite;
using Xamarin.Forms;
using Application = Android.App.Application;
using Environment = System.Environment;

[assembly: Dependency(typeof(DomoticaProject.Droid.DBProvider))]
namespace DomoticaProject.Droid {
    public class DBProvider : IDBAccess {
        public SQLiteConnection Connect() {
            var file = "LuaController.db";
            var path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Personal), file);

            if (!File.Exists(path)) {
                using (var br = new BinaryReader(Application.Context.Assets.Open(file))) {
                    using (var bw = new BinaryWriter(new FileStream(path, FileMode.Create))) {
                        byte[] buffer = new byte[4096];
                        int len = 0;

                        while ((len = br.Read(buffer, 0, buffer.Length)) > 0) bw.Write(buffer, 0, len);
                    }
                }
            }

            var con = new SQLiteConnection(path, false);

            con.Execute(@"
                CREATE TABLE IF NOT EXISTS FileStorage (
                    ID Text PRIMARY KEY NOT NULL UNIQUE,
                    Contents Blob NOT NULL
                );
            ");

            con.Execute(@"
                CREATE TABLE IF NOT EXISTS AttachableAlias (
                    Name Text PRIMARY KEY NOT NULL UNIQUE,
                    Alias Text NOT NULL UNIQUE
                );
            ");

            return con;
        }
    }
}