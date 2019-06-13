using DomoticaProject.Server;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using Command = DomoticaProject.Server.Command;

namespace DomoticaProject {
    public partial class MainPage : ContentPage {
        private ConnectionManager mgr;
        private ServerCommandManager cmd;

        private ObservableCollection<string> ResponseText;

        public MainPage() {
            InitializeComponent();

            this.mgr = new ConnectionManager(IPAddress.Parse("192.168.178.31"), 5300);
            this.cmd = new ServerCommandManager(mgr);

            ResponseText = new ObservableCollection<string>();

            IReadOnlyList<Command> cmds = cmd.GetEnabledCommands(TimeSpan.FromMilliseconds(5000));
            ObservableCollection<Command> src = new ObservableCollection<Command>(cmds);

            CommandList.ItemsSource = src;
            ResponseList.ItemsSource = ResponseText;
        }

        private void CommandItemTapped(object sender, EventArgs e) {
            Command tappedcmd = (Command) ((ListView) sender).SelectedItem;

            new Task(() => {
                List<string> response = cmd.SendCommand(tappedcmd, TimeSpan.FromMilliseconds(3500));
                if (response == null) {
                    ResponseText.Clear();

                    ResponseText.Add("< Connection Timeout >");
                } else {
                    ResponseText.Clear();
                    foreach (string str in response) ResponseText.Add(str);
                }
            }).Start();
        }
    }
}
