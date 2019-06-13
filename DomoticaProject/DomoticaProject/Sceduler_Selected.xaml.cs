using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace DomoticaProject
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class Sceduler_Selected : ContentPage
    {
        public Sceduler_Selected(object arduinoSelected)
        {
            InitializeComponent();
            arduino.Text = arduinoSelected.ToString();

        }
    }
}