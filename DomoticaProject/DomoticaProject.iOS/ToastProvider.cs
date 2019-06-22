using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using DomoticaProject.iOS;
using DomoticaProject.Utility;
using Foundation;
using UIKit;
using Xamarin.Forms;

[assembly: Dependency(typeof(DomoticaProject.iOS.ToastProvider))]
namespace DomoticaProject.iOS {
    public class ToastProvider : IToast {
        private const double LONG_DELAY = 3.5;

        private NSTimer alertDelay;
        private UIAlertController alert;

        public void show(string message) {
            alertDelay = NSTimer.CreateScheduledTimer(LONG_DELAY, (obj) => {
                if (alert != null) {
                    alert.DismissViewController(true, null);
                }

                if (alertDelay != null) {
                    alertDelay.Dispose();
                }
            });

            alert = UIAlertController.Create(null, message, UIAlertControllerStyle.Alert);
            UIApplication.SharedApplication.KeyWindow.RootViewController.PresentViewController(alert, true, null);
        }
    }
}