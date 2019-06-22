using DomoticaProject.Lua.MoonSharp;
using DomoticaProject.Utility;
using Plugin.FilePicker;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject.MVVM {
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class LuaEditor : ContentPage {
        private string title;
        private bool changes;

        public LuaEditor() {
            InitializeComponent();

            this.changes = SettingsManager.instance["EditedScript"] != null;
            this.title = "unnamed";

            Editor.Text = SettingsManager.instance["EditedScript"];
        }

        protected override void OnDisappearing() {
            base.OnDisappearing();
            SettingsManager.instance["EditedScript"] = Editor.Text;
        }

        private void SaveButtonClicked(object sender, EventArgs e) {
            string[] script = Editor?.Text.Split('\n') ?? new string[0];

            // Open save file dialogue. If it completes, write to file.
            Navigation.PushModalAsync(new SaveLuaPage(
                title,
                (string file) => {
                    title = file;

                    string dest = "Scripts/" + file + ".lua";
                    FileAccessProvider.WriteText(dest, script);

                    changes = false;
                },
                () => { }
            ));
        }

        private async void OpenButtonClicked(object sender, EventArgs e) {
            // Remind to save before closing current script.
            if (changes) {
                bool ShouldSave = await DisplayAlert("Save first?", "Do you want to save your progress before leaving?", "Yes", "No");
                if (ShouldSave) SaveButtonClicked(null, null);
            }

            await Navigation.PushModalAsync(new OpenLuaPage((string title, string[] contents) => {
                this.title = title;
                Editor.Text = string.Join("\n", contents);
                changes = false;
            }));
        }

        private void PlayButtonClicked(object sender, EventArgs e) {
            // Run script and handle exceptions.
            ScriptExecuter.Execute(
                Editor.Text.Split('\n').ToList(),
                (ScriptExecuter.ScriptErrorType type, string message) => {
                    // On exception, show toast message and log it.
                    switch (type) {
                        case ScriptExecuter.ScriptErrorType.SYNTAX_ERROR:
                            ToastProvider.ShowToast("Syntax error in script. See logfile for details.");
                            LoggerPage.instance.LogMessage("Syntax error in script: \n" + message, LoggerPage.MessageType.ERROR);
                            break;
                        case ScriptExecuter.ScriptErrorType.RUNTIME_ERROR:
                            ToastProvider.ShowToast("Error while executing script. See logfile for details.");
                            LoggerPage.instance.LogMessage("Error while executing script: \n" + message, LoggerPage.MessageType.ERROR);
                            break;
                        case ScriptExecuter.ScriptErrorType.INTERNAL_ERROR:
                            ToastProvider.ShowToast("Internal error in scripting engine. See logfile for details.");
                            LoggerPage.instance.LogMessage("Internal error while executing script: \n" + message, LoggerPage.MessageType.ERROR);
                            break;
                        case ScriptExecuter.ScriptErrorType.CONNECTION_ERROR:
                            ToastProvider.ShowToast("No connection to Arduino. See logfile for details.");
                            LoggerPage.instance.LogMessage("Script execution was stopped because there was no Arduino connection: \n" + message, LoggerPage.MessageType.WARNING);
                            break;
                    }

                    LoggerPage.instance.LogMessage("While executing script " + title + ".lua");
                }
            );
        }
    }
}