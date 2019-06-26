using SQLite;
using System;
using System.Collections.Generic;
using System.Text;

namespace DomoticaProject.Utility {
    public static class DBUtils {
        public class AtblAlias {
            [PrimaryKey, NotNull, Unique] public string Name { get; set; }
            [NotNull, Unique] public string Alias { get; set; }
        }

        private static SQLiteConnection Connect() {
            return DBProvider.Connect();
        }

        public static AtblAlias GetAlias(string name) {
            var conn = Connect();
            return conn.FindWithQuery<AtblAlias>("SELECT * FROM AttachableAlias WHERE Name = ?", name);
        }

        public static void SetAlias(AtblAlias alias) {
            var conn = Connect();
            conn.Execute("INSERT OR REPLACE INTO AttachableAlias VALUES (?, ?)", alias.Name, alias.Alias);
        }

        public static List<AtblAlias> GetAliases() {
            var conn = Connect();
            return conn.Query<AtblAlias>("SELECT * FROM AttachableAlias");
        }
    }
}
