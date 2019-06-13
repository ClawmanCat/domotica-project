using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject
{
    public partial class Sceduler : ContentPage
    {
        ObservableCollection<ScedulerArduino> arduinos = new ObservableCollection<ScedulerArduino>();
        public Sceduler()
        {

            InitializeComponent();
            arduinos.Add(new ScedulerArduino() { Name = "NameExample1", Description = "DescriptionExample1" });
            arduinos.Add(new ScedulerArduino() { Name = "NameExample2", Description = "DescriptionExample2" });
            arduinos.Add(new ScedulerArduino() { Name = "NameExample3", Description = "DescriptionExample3" });
            arduinos.Add(new ScedulerArduino() { Name = "NameExample4", Description = "DescriptionExample4" });
            ArduinoList.ItemsSource = arduinos.ToList();
            

        }
        
private void ArduinoList_ItemTapped(object sender, ItemTappedEventArgs e)
        {
            int itemsel = e.ItemIndex;
            Navigation.PushAsync(new Sceduler_Selected(arduinos[itemsel].Name));
        }
    }
}