using Plugin.Settings;
using Plugin.Settings.Abstractions;
using System;
using System.Collections.Generic;
using System.Text;

namespace DomoticaProject.Utility {
    public class SettingsManager {
        public static SettingsManager instance = new SettingsManager();

        private SettingsManager() { }

        private static ISettings AppSettings = CrossSettings.Current;

        public string this[string key] {
            get => AppSettings.GetValueOrDefault(key, (string) null);
            set => AppSettings.AddOrUpdateValue(key, value);
        }

        public string GetOrDefault(string key, string def) {
            return AppSettings.GetValueOrDefault(key, def);
        }
    }
}
