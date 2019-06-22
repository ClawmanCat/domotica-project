using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace DomoticaProject.Utility {
    public interface IToast {
        void show(string message);
    }

    public static class ToastProvider {
        public static void ShowToast(string message) {
            DependencyService.Get<IToast>().show(message);
        }
    }
}
