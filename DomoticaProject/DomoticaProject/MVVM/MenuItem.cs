using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using Xamarin.Forms;

namespace DomoticaProject.MVVM {
    class MenuItem {
        public delegate Page OnItemClicked();
        public delegate bool ShouldShowItem();

        public string Icon { get; set; }
        public string Text { get; set; }
        public OnItemClicked OnClicked;
        public ShouldShowItem ShouldShow;
    }
}
