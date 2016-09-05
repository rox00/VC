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

namespace UYouMain
{
    /// <summary>
    /// InstallAPP.xaml 的交互逻辑
    /// </summary>
    public partial class InstallAPP : Window
    {
        public InstallAPP()
        {
            InitializeComponent();
        }

        public void InstallBegin()
        {
            ShowTipBlock.Text = "正在安装应用，请稍后！";
            EndInstallBtn.Visibility = Visibility.Hidden;
            Cursor = Cursors.Wait;
        }

        public void InstallSuccess()
        {
            ShowTipBlock.Text = "安装成功！";
            EndInstallBtn.Visibility = Visibility.Visible;
            Cursor = Cursors.Arrow;
        }

        public void EndInstall()
        {
            Close();
        }

        private void EndInstallBtn_Click(object sender, RoutedEventArgs e)
        {
            EndInstall();
        }      
        private void Window_Activated(object sender, EventArgs e)
        {
            var window = sender as Window;
            //window.Topmost = true;
        }

        private void Window_Deactivated(object sender, EventArgs e)
        {
            var window = sender as Window;
            //window.Topmost = false;
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.DragMove();
            }

        }  
    }
}
