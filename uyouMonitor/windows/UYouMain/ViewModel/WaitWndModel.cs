using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using UYouMain.Common;

namespace UYouMain.ViewModel
{
    class WaitWndModel : NotificationObject
    {

        /****************************************************/
        private Visibility wndVisible;

        public Visibility WndVisible
        {
            get { return wndVisible; }
            set
            {
                wndVisible = value;
                this.RaisePropertyChanged("WndVisible");
            }
        }
        /****************************************************/

        /****************************************************/
        private double                          windowLeft;
        public double WindowLeft
        {
            get { return windowLeft; }
            set
            {
                windowLeft = value;
                this.RaisePropertyChanged("WindowLeft");
            }
        }
        /****************************************************/

        /****************************************************/
        private double                          windowTop;
        public double WindowTop
        {
            get { return windowTop; }
            set
            {
                windowTop = value;
                this.RaisePropertyChanged("WindowTop");
            }
        }
        /****************************************************/

        /****************************************************/
        private double                          windowWidth;
        public double WindowWidth
        {
            get { return windowWidth; }
            set
            {
                windowWidth = value;
                this.RaisePropertyChanged("WindowWidth");
            }
        }
        /****************************************************/

        /****************************************************/
        private double                          windowHeight;
        public double WindowHeight
        {
            get { return windowHeight; }
            set
            {
                windowHeight = value;
                this.RaisePropertyChanged("WindowHeight");
            }
        }
        /****************************************************/

        public WaitWndModel()
        {
            wndVisible                      = Visibility.Hidden;

            windowLeft                      = 0;
            windowTop                       = 0;
            windowWidth                     = 0;
            windowHeight                    = 0;
        }

        public void ExitWnd(object obj)
        {
            ////清除热键
            //HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            //if (hwndSource != null)
            //    Common.Win32APIs.UnregisterHotKey(hwndSource.Handle, hwndSource.Handle.ToInt32());

            //try
            //{
            //    ////先清除弹出窗口
            //    if (_vedioListWnd != null)
            //    {
            //        _vedioListWnd.Hide();
            //        _vedioListWnd.Close();
            //        _vedioListWnd = null;
            //    }

            //    _notifyIcon.Dispose();

            //    _axSVCBoxMonitor.ClearTouchedKey();
            //    _axSVCBoxMonitor.Exit();                    ////控件优先退出

            //    SaveConfig();
            //}
            //catch (Exception ex)
            //{
            //    log.Error(ex.Message);
            //}
        }
    }
}
