using DomoticaProject.Lua.MoonSharp;
using DomoticaProject.Server;
using System;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

[assembly: XamlCompilation(XamlCompilationOptions.Compile)]
namespace DomoticaProject {
    public partial class App : Application {
        public static ConnectionManager ArduinoConnection = null;

        public App() {
            InitializeComponent();
            MainPage = new MVVM.MasterPage();
        }

        protected override void OnStart() {
            APIRegistry.RegisterAll();
        }

        protected override void OnSleep() {
            // Handle when your app sleeps
        }

        protected override void OnResume() {
            // Handle when your app resumes
        }
    }
}
