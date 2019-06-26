using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject.MVVM {
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class AddAliasPage : ContentPage {
        public delegate void Callback(string NewName);

        public string CurrentAlias { get; set; }
        private Callback callback;

		public AddAliasPage(string name, Callback cb) {
            InitializeComponent();

            this.callback = cb;
            this.CurrentAlias = name;
		}

        private void DoneClicked(object sender, EventArgs e) {
            callback(AliasEntry.Text);
            Navigation.PopModalAsync();
        }
    }
}