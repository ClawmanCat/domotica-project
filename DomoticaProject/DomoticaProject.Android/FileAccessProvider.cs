using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Android;
using Android.App;
using Android.Content;
using Android.Content.PM;
using Android.OS;
using Android.Runtime;
using Android.Support.V4.App;
using Android.Support.V4.Content;
using Android.Views;
using Android.Widget;
using DomoticaProject.Utility;
using SQLite;
using Xamarin.Forms;
using Application = Android.App.Application;
using Environment = System.Environment;

[assembly: Dependency(typeof(DomoticaProject.Droid.FileAccessProvider))]
namespace DomoticaProject.Droid {
    // Android for some reason refuses to give file io access, even with the correct permissions set.
    // Reading and writing from and to the database seems allowed though, so just store the files in a
    // database for now.
    public class FileAccessProvider : IFileAccess {
        public byte[] ReadFromFile(string file) {
            var con = GetDBConnection();

            DBFile f = con.FindWithQuery<DBFile>("SELECT * FROM FileStorage WHERE ID=?", file);
            return f?.Contents ?? null;
        }

        public void WriteToFile(string file, byte[] data) {
            var con = GetDBConnection();

            con.Execute("INSERT INTO FileStorage VALUES (?, ?)", file, data);
        }

        public string[] ListFiles(string folder) {
            var con = GetDBConnection();

            // LIKE-clause will remove most non-matching files, but some will have to be removed by regex.
            List<DBFile> results = con.Query<DBFile>("SELECT * FROM FileStorage WHERE ID LIKE ?", folder + "%");
            return results.Where((DBFile f) => { return f.ID.ContainsRegex("^" + Regex.Escape(folder) + @"\/[^\/]+\..+"); }).Select(f => f.ID).ToArray();
        }


        private class DBFile {
            [PrimaryKey, NotNull, Unique] public string ID { get; set; }
            [NotNull] public byte[] Contents { get; set; }
        }

        private SQLiteConnection GetDBConnection() {
            return new DBProvider().Connect();
        }
    }
}