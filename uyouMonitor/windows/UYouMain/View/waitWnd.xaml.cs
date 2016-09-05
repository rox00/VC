using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace UYouMain
{
    /// <summary>
    /// waitWnd.xaml 的交互逻辑
    /// </summary>
    public partial class waitWnd : Window
    {
        public waitWnd()
        {
            InitializeComponent();
        }
        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            if (hwndSource != null)
            {
                hwndSource.AddHook(new HwndSourceHook(this.WndProc));
            }
        }
        protected virtual IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch (msg)
            {
                case 0x0084://WM_NCHITTEST
                        handled = true;
                    return new IntPtr((int)Common.HitTest.HTTRANSPARENT);
            }
            return IntPtr.Zero;
        }
    }
}
