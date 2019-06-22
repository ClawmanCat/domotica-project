using DomoticaProject.Utility;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject.MVVM {
	[XamlCompilation(XamlCompilationOptions.Compile)]
	public partial class LoggerPage : ContentPage {
        public static LoggerPage instance = new LoggerPage();
        public enum MessageType { INFO, WARNING, ERROR };

        private ObservableCollection<LoggerMessage> contents;

		public LoggerPage() {
			InitializeComponent();

            this.contents = new ObservableCollection<LoggerMessage>();
            LoggerList.ItemsSource = this.contents;

            LogMessage("Initialized logger.");
		}

        public void LogMessage(string message, MessageType type = MessageType.INFO) {
            string result = "[" + DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss") + "] " + type.ToString() + ": " + message;

            Console.WriteLine(result);
            contents.Add(new LoggerMessage {
                Text = result,
                TextColor = (type == MessageType.ERROR ? Color.Red : (type == MessageType.WARNING ? Color.Orange : Color.Black))
            });
        }
	}
}