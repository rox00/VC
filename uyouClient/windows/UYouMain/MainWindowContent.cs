using System;
using System.Globalization;
using System.Windows;
using System.Windows.Input;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Management;
using System.Diagnostics;
using System.Windows.Data;
using System.Windows.Interop;
using System.Windows.Navigation;

namespace UYouMain
{
    
    public partial class MainWindowContent : ResourceDictionary
    {        
        private NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        [DllImport("user32.dll",CharSet = CharSet.Auto, SetLastError = true)]
        static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, uint wParam, uint lParam);
        [DllImport("user32.dll",CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr GetForegroundWindow();

        public MainWindowContent()
        {
            InitializeComponent();
        }

        
        public void addVol(){
            log.Info("sendmessage voice add");
            SendMessage(GetForegroundWindow(), WM_APPCOMMAND, 0x30292, APPCOMMAND_VOLUME_UP * 0x10000);
        }

        public void subVol()
        {
            log.Info("sendmessage voice sub");
            SendMessage(GetForegroundWindow(), WM_APPCOMMAND, 0x30292, APPCOMMAND_VOLUME_DOWN * 0x10000);
        }
        private void Hyperlink_RequestNavigate(object sender, RequestNavigateEventArgs e)
        {

            Process.Start(new ProcessStartInfo(e.Uri.AbsoluteUri));

            e.Handled = true;

        }
        const uint WM_APPCOMMAND = 0x319;
        const uint APPCOMMAND_VOLUME_UP = 0x0a;
        const uint APPCOMMAND_VOLUME_DOWN = 0x09;
        const uint APPCOMMAND_VOLUME_MUTE = 0x08;
    }    
}
