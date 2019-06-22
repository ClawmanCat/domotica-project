using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using DomoticaProject.Droid;
using DomoticaProject.Utility;
using Xamarin.Forms;

[assembly: Dependency(typeof(DomoticaProject.Droid.ToastProvider))]
namespace DomoticaProject.Droid {
    public class ToastProvider : IToast {
        public void show(string message) {
            Toast.MakeText(Android.App.Application.Context, message, ToastLength.Long).Show();
        }
    }
}