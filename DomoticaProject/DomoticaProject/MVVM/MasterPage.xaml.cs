using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject.MVVM {
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class MasterPage : MasterDetailPage {
        private List<MenuItem> GlobalMenuContents = new List<MenuItem> {
            new MenuItem {
                Icon = Convert.ToChar(0xF002).ToString(),
                Text = "Connect to Arduino",
                OnClicked  = () => { return ConnectionPage.instance; },
                ShouldShow = () => { return true; }
            },

            new MenuItem {
                Icon = Convert.ToChar(0xFA37).ToString(),
                Text = "List Commands",
                OnClicked  = () => { return new CommandPage(); },
                ShouldShow = () => { return App.ArduinoConnection?.IsConnected() ?? false; }
            }
        };

        private ObservableCollection<MenuItem> MenuContents;
        private Timer MenuUpdateTimer;

		public MasterPage() {
			InitializeComponent();

            Detail = new NavigationPage(new WelcomePage());

            this.MenuContents = new ObservableCollection<MenuItem>();
            MasterMenuItems.ItemsSource = MenuContents;

            // Make sure only the items that should be shown will appear in the menu.
            MenuUpdateTimer = new Timer { AutoReset = true, Interval = 500 };
            MenuUpdateTimer.Elapsed += (object source, ElapsedEventArgs args) => {
                Device.BeginInvokeOnMainThread(() => {
                    MenuContents.Clear();

                    foreach (MenuItem m in GlobalMenuContents) {
                        if (m.ShouldShow()) MenuContents.Add(m);
                    }
                });
            };

            MenuUpdateTimer.Start();
		}

        private void MenuItemTapped(object sender, ItemTappedEventArgs e) {
            MenuItem m = (MenuItem) e.Item;
            if (m.ShouldShow()) Detail = new NavigationPage(m.OnClicked());
        }
    }
}