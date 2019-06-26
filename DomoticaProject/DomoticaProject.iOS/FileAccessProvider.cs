using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DomoticaProject.Utility;
using Foundation;
using UIKit;
using Xamarin.Forms;

[assembly: Dependency(typeof(DomoticaProject.iOS.FileAccessProvider))]
namespace DomoticaProject.iOS {
    class FileAccessProvider : IFileAccess {
        public string[] ListFiles(string folder) {
            throw new NotImplementedException();
        }

        public byte[] ReadFromFile(string file) {
            throw new NotImplementedException();
        }

        public void WriteToFile(string file, byte[] data) {
            throw new NotImplementedException();
        }
    }
}