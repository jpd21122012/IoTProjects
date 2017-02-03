using Mfrc522Lib;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.Devices.Gpio;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;

namespace IoT_rfid
{
    public sealed partial class MainPage : Page
    {
        private DispatcherTimer timer = new DispatcherTimer();
        private Mfrc522 rfid = new Mfrc522();
        private string[] usersID = new string[] {"01c3dcd6", "a5ef4d27" };
        private string[] usersName = new string[] { "Jorge Perales Díaz", "Laura Pamela Aguirre García" };
        private string[] usersImage = new string[] { "User1.jpg", "User2.png"};
        public MainPage()
        {
            this.InitializeComponent();
        }
        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            await rfid.InitIO();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += temporizador_Tick;
            timer.Start();
        }

        private void temporizador_Tick(object sender, object e)
        {
            LectorRFID();
        }

        private void LectorRFID()
        {
            while (true)
            {
                if (rfid.IsTagPresent())
                {
                    var uniqueID = rfid.ReadUid();
                    Debug.WriteLine(uniqueID.ToString());
                    rfid.HaltTag();
                    for (int i = 0; i < usersID.Length; i++)
                    {
                        if (uniqueID.ToString() == usersID[i])
                        {
                            textBoxUser.Text = string.Format("{0}", usersName[i].ToString());
                            imageUser.Source = new BitmapImage(new Uri("ms-appx:///Assets/User/" + usersImage[i]));
                        }
                    }
                    break;
                }
            }
        }
    }
}

