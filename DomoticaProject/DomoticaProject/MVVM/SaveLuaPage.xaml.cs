using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

using DomoticaProject.Utility;

namespace DomoticaProject.MVVM {
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class SaveLuaPage : ContentPage {
        public delegate void OnSave(string file);
        public delegate void OnCancel();

        private OnSave SaveCallback;
        private OnCancel CancelCallback;

		public SaveLuaPage(string DefaultName, OnSave SaveCallback, OnCancel CancelCallback) {
			InitializeComponent();

            this.SaveCallback = SaveCallback;
            this.CancelCallback = CancelCallback;

            FilenameEntry.Text = DefaultName;
		}

        private void SaveButtonClicked(object sender, EventArgs e) {
            if (string.IsNullOrWhiteSpace(FilenameEntry.Text) || !FilenameEntry.Text.IsValidFilename()) return;

            SaveCallback(FilenameEntry.Text);
            Navigation.PopModalAsync();
        }

        private void CancelButtonClicked(object sender, EventArgs e) {
            CancelCallback();
            Navigation.PopModalAsync();
        }
    }
}