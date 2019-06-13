using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DomoticaProject.Utility {
    static class Extensions {
        // Split each string in the list over the first occurrence of the seperator, and store them split up in a new list.
        public static List<(string, string)> SplitPairs(this IEnumerable<string> list, char seperator) {
            List<(string, string)> result = new List<(string, string)>();

            foreach (string str in list) {
                List<string> split = new List<string>(str.Split(seperator));

                result.Add((
                    split[0],
                    string.Join("", split.GetRange(1, split.Count - 1))
                ));
            }

            return result;
        }

        // Get part of an array.
        public static IEnumerable<T> Subset<T>(this T[] array, int begin, int count) {
            return array.Skip(begin).Take(count);
        }
    }
}
