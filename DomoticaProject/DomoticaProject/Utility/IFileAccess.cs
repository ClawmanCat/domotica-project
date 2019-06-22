using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace DomoticaProject.Utility {
    public interface IFileAccess {
        void WriteToFile(string file, byte[] data);
        byte[] ReadFromFile(string file);
        string[] ListFiles(string folder);
    }

    public static class FileAccessProvider {
        public static void WriteData(string file, byte[] data) {
            DependencyService.Get<IFileAccess>().WriteToFile(file, data);
        }

        public static byte[] ReadData(string file) {
            return DependencyService.Get<IFileAccess>().ReadFromFile(file);
        }

        public static string[] ListFiles(string folder) {
            return DependencyService.Get<IFileAccess>().ListFiles(folder);
        }

        public static void WriteText(string file, string[] data) {
            FileAccessProvider.WriteData(
                file,
                Encoding.ASCII.GetBytes(string.Join("\n", data))    
            );
        }

        public static string[] ReadText(string file) {
            return Encoding.ASCII.GetString(FileAccessProvider.ReadData(file)).Split('\n');
        }
    }
}
