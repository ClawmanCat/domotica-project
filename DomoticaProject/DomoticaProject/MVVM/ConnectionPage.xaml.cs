using DomoticaProject.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject.MVVM {
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class ConnectionPage : ContentPage {
        public static ConnectionPage instance = new ConnectionPage();

        // Unicode codes for the Material Design icon font.
        // Lookup table: https://cdn.materialdesignicons.com/3.6.95/
        private static char FAILURE_SYMBOL = Convert.ToChar(0xF739);
        private static char SUCCESS_SYMBOL = Convert.ToChar(0xF12C);
        private static char WAITING_SYMBOL = Convert.ToChar(0xF1D8);

        private Timer  PrevTimer = null;
        private string PrevStatus = null;

        public ConnectionPage() {
			InitializeComponent();

            // Auto-fill last connection info, if it exists.
            var Props = Application.Current.Properties;
            if (Props.ContainsKey("LastIP"))   IPEntry.Text   = Props["LastIP"]   as string;
            if (Props.ContainsKey("LastPort")) PortEntry.Text = Props["LastPort"] as string;
        }

        private void ConnectClicked(object sender, EventArgs e) {
            IPAddress address;
            int port;

            if (IPAddress.TryParse(IPEntry.Text, out address) && Int32.TryParse(PortEntry.Text, out port)) {
                if (App.ArduinoConnection?.GetConnectionAddress().Equals((address, port)) ?? false) return;

                // Begin connecting and end the previous connection if one exists.
                App.ArduinoConnection?.Shutdown();
                App.ArduinoConnection = new ConnectionManager(address, port);

                SetFeedback(WAITING_SYMBOL, "Connecting...");

                // Check if there is a connection repeatedly. If there is, update the message.
                Timer ConnectionChecker = new Timer { Interval = 500, AutoReset = true };
                ConnectionChecker.Elapsed += (object source, ElapsedEventArgs args) => {
                    if (App.ArduinoConnection.IsConnected()) {
                        SetFeedback(SUCCESS_SYMBOL, "Connection established with Arduino.");

                        // Save the connection info for next time.
                        var Props = Application.Current.Properties;
                        Props["LastIP"]   = address.ToString();
                        Props["LastPort"] = port.ToString();
                    } else {
                        SetFeedback(WAITING_SYMBOL, "Connecting...");
                    }
                };

                // If there was already a connection checker running from last time the button was pressed,
                // stop it and start using this one instead.
                PrevTimer?.Stop();
                PrevTimer = ConnectionChecker;

                PrevTimer.Start();
            } else {
                SetFeedback(FAILURE_SYMBOL, "Invalid IP address and/or port.");
            }
        }

        private void SetFeedback(char symbol, string message) {
            // Dont change if the new message is the same as the current message.
            if (message == PrevStatus) return;
            else PrevStatus = message;

            // Simply editing existing labels does not recenter them,
            // so they have to be created anew on every change.

            Label icon = new Label {
                FontSize = Device.GetNamedSize(NamedSize.Large, typeof(Label)),
                FontFamily = (OnPlatform<string>) App.Current.Resources["IconFont"],
                HorizontalOptions = LayoutOptions.CenterAndExpand,
                VerticalOptions = LayoutOptions.Center,
                TextColor = (symbol == SUCCESS_SYMBOL ? Color.Green : (symbol == FAILURE_SYMBOL ? Color.Red : Color.Gray)),
                Text = symbol.ToString()
            };

            Label text = new Label {
                HorizontalOptions = LayoutOptions.CenterAndExpand,
                VerticalOptions = LayoutOptions.Center,
                Text = message
            };

            Device.BeginInvokeOnMainThread(() => {
                Status.Children.Clear();
                Status.Children.Add(icon);
                Status.Children.Add(text);
            });
        }
    }
}