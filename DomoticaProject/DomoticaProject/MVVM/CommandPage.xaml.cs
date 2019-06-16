using DomoticaProject.Server;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Command = DomoticaProject.Server.Command;

namespace DomoticaProject.MVVM {
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class CommandPage : ContentPage {
        private ObservableCollection<Command> commands;
        private bool IsShown;
        private Action GetCommandList;

		public CommandPage() {
			InitializeComponent();

            IsShown = true; // Only update list if it is visible.

            commands = new ObservableCollection<Command>();
            CommandList.ItemsSource = commands;
            
            // Attempt to fetch the command list.
            // If successful, fill the list, else display an error message.
            GetCommandList = () => {
                App.ArduinoConnection.CommandManager.GetEnabledCommandsAsync(
                    (IReadOnlyList<Command> cmds) => {
                        Device.BeginInvokeOnMainThread(() => {
                            ResponseDisplay.Text = "";

                            commands.Clear();
                            foreach (Command c in cmds) commands.Add(c);
                        });
                    },
                    () => {
                        Device.BeginInvokeOnMainThread(() => {
                            ResponseDisplay.Text = "< No connection. Attempting to re-establish... >";
                            if (IsShown) GetCommandList();
                        });
                    },
                    TimeSpan.FromMilliseconds(2500)
                );
            };

            GetCommandList();
		}

        private void CommandTapped(object sender, ItemTappedEventArgs e) {
            // Send the command to the Arduino.
            Command cmd = (Command) e.Item;
            App.ArduinoConnection.CommandManager.SendCommandAsync(
                cmd,
                (List<string> response) => {
                    Device.BeginInvokeOnMainThread(() => {
                        // Show response on screen
                        ResponseDisplay.Text = string.Join("\n", response);
                    });
                },
                () => {
                    Device.BeginInvokeOnMainThread(() => {
                        // Show error message
                        ResponseDisplay.Text = "< Response timed out >";
                    });
                },
                TimeSpan.FromMilliseconds(2500)
            );
        }

        protected override void OnAppearing() {
            base.OnAppearing();
            IsShown = true;
            GetCommandList();
        }

        protected override void OnDisappearing() {
            base.OnDisappearing();
            IsShown = false;
        }
    }
}