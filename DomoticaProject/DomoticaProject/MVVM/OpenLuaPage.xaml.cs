using DomoticaProject.Utility;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject.MVVM {
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class OpenLuaPage : ContentPage {
        public delegate void OnFileChosen(string title, string[] contents);

        private class ListItem {
            public string Title { get; set; }
            public string Desc { get; set; }

            public string[] contents;
        }

        private OnFileChosen callback;

        public OpenLuaPage(OnFileChosen callback) {
            InitializeComponent();

            this.callback = callback;

            List<ListItem> items = new List<ListItem>();
            foreach (string file in FileAccessProvider.ListFiles("Scripts")) {
                string[] contents = FileAccessProvider.ReadText(file);
                string mgcontents = string.Join(" ", contents);

                items.Add(new ListItem {
                    Title = file.Split('/').Last(),
                    Desc  = mgcontents.Length > 30 ? (mgcontents.Substring(0, 27) + "...") : mgcontents,
                    contents = contents
                });
            }

            FileList.ItemsSource = items;
        }

        private void FileSelected(object sender, SelectedItemChangedEventArgs e) {
            ListItem item = (ListItem) e.SelectedItem;

            callback(item.Title, item.contents);
            Navigation.PopModalAsync();
        }
    }
}