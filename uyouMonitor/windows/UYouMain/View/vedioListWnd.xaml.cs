using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml;
using UYouMain.Common;

namespace UYouMain
{
    /// <summary>
    /// vedioListWnd.xaml 的交互逻辑
    /// </summary>
    public partial class vedioListWnd : Window
    {
        /*********************Window事件*****************************/
        public vedioListWnd()
        {
            InitializeComponent();
            this.Closing            += vedioListWnd_Closing;
        }

        private void vedioListWnd_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (Visibility == Visibility.Visible)
            {
                e.Cancel = true;

                this.Hide();
            }
        }
    }
}
