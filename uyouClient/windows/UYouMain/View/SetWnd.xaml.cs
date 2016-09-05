using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using UYouMain.ViewModel;

namespace UYouMain
{
    /// <summary>
    /// SetWnd.xaml 的交互逻辑
    /// </summary>
    public partial class SetWnd : Window
    {
        public SetWnd()
        {
            InitializeComponent();

            this.MouseMove      += SetWnd_MouseMove;
        }

        void SetWnd_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
                this.DragMove();
        }
    }
}
