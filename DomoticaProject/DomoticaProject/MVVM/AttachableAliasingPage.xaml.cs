using DomoticaProject.Lua.MoonSharp;
using DomoticaProject.Server;
using DomoticaProject.Utility;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using static DomoticaProject.Utility.DBUtils;

namespace DomoticaProject.MVVM {
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class AttachableAliasingPage : ContentPage {
        private class AliasListItem {
            public string Name { get; set; }
            public string Alias { get; set; }

            public ArduinoAttachable attachable;

            public string AttLocation {
                get => "@" + attachable.device + ":" + attachable.port;
            }

            public bool IsBadAlias {
                get => Alias.Contains(" ");
            }
        }

        public static AttachableAliasingPage instance = new AttachableAliasingPage();
        private ObservableCollection<AliasListItem> items = new ObservableCollection<AliasListItem>();

        public AttachableAliasingPage() {
            InitializeComponent();
        }


        public void init() {
            items.Clear();

            var dbaliases = DBUtils.GetAliases();
            var attachables = App.ArduinoConnection.AttachableManager.GetAttachables();

            // For each attachable, find its alias or create a new one.
            // Add it to the shown list and register it as callable in the API.
            foreach (var attachable in attachables) {
                AtblAlias FoundAlias = null;

                // Find alias for attachable
                foreach (var alias in dbaliases) {
                    if (attachable.name == alias.Name) {
                        FoundAlias = alias;
                        continue;
                    }
                }

                // Not found, make a new one.
                if (FoundAlias == null) {
                    FoundAlias = new AtblAlias {
                        Name = attachable.name,
                        Alias = attachable.name
                    };
                }

                // Add to displayed items.
                items.Add(new AliasListItem
                {
                    Name = FoundAlias.Name,
                    Alias = FoundAlias.Alias,
                    attachable = attachable
                });

                // Register to API.
                APIRegistry.RegisterMethod(
                    (string data) => {
                        return App.ArduinoConnection.AttachableManager.RunAttachable(attachable, data);
                    },
                    FoundAlias.Alias
                );
            }

            AttachableList.ItemsSource = items;
        }


        private void AttachableItemTapped(object sender, ItemTappedEventArgs e) {
            AliasListItem item = (AliasListItem) e.Item;

            // Open screen to type in a new alias,
            // when the new alias is set, update the shown list and the API.
            Navigation.PushModalAsync(new AddAliasPage(
                item.Name == item.Alias ? "" : item.Alias,
                (string NewName) => {
                    // Remove old value from API.
                    APIRegistry.RemoveMethod(item.Alias);

                    // Update item & shown list.
                    int index = items.IndexOf(item);

                    items.RemoveAt(index);
                    item.Alias = NewName;
                    items.Insert(index, item);

                    // Add new value to API.
                    APIRegistry.RegisterMethod(
                        (string data) => {
                            return App.ArduinoConnection.AttachableManager.RunAttachable(item.attachable, data);
                        },
                        item.Alias
                    );
                }
            ));
        }
    }
}