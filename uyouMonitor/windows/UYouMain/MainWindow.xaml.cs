using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;
using System.Windows.Resources;

using System.Runtime.InteropServices;   //互动服务 
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;

using System.Threading;
using System.Timers;
using System.Reflection;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using NLog;
using System.Diagnostics;
using System.Resources;
using System.Drawing;
using System.Net;
using UYouMain.ViewModel;
using System.Windows.Forms.Integration;

namespace UYouMain
{    

    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        System.Windows.Point[] VedioSamplePointList =
	                                                {	//暂时取以下采样点
		                                                new System.Windows.Point( 100, 100 ),
		                                                new System.Windows.Point( 200, 200 ),
		                                                new System.Windows.Point( 200, 100 ),
		                                                new System.Windows.Point( 100, 200 ),
		                                                new System.Windows.Point( 300, 300 ),
		                                                new System.Windows.Point( 100, 300 ),
		                                                new System.Windows.Point( 300, 100 ),
		                                                new System.Windows.Point( 200, 300 ),
		                                                new System.Windows.Point( 300, 200 ),
	                                                };

        Logger log = LogManager.GetCurrentClassLogger();
        private System.Windows.Forms.NotifyIcon     _notifyIcon         = null;

        private AxSVCBoxMonitorLib.AxSVCBoxMonitor  _axSVCBoxMonitor    = new AxSVCBoxMonitorLib.AxSVCBoxMonitor();
        private KeyMapping                          _keyMap             = new KeyMapping();
        private waitWnd                             _waitWnd            = new waitWnd();
        private vedioListWnd                        _vedioListWnd       = new vedioListWnd();

        MainWindowModel                             _mainWndowModel     = null;

        public MainWindow()
        {
            InitializeComponent();
            InitialTray();

            WindowsFormsHost host       = this.TryFindResource("Root") as WindowsFormsHost;
            host.Child                  = _axSVCBoxMonitor;
            wndContent.Content          = host;
            _mainWndowModel             = this.TryFindResource("ViewModel_MainWindowModel") as MainWindowModel;
            InitWndEvent();
        }
        /*********************自定义函数*****************************/
        private void InitialTray()
        {

            //设置托盘的各个属性
            _notifyIcon                  = new System.Windows.Forms.NotifyIcon();
            _notifyIcon.BalloonTipText   = "U游安卓世界";
            _notifyIcon.Text             = "托盘图标";
            _notifyIcon.Icon             = new Icon(Application.GetResourceStream(new Uri("/Resources/imgs/black/logo.ico", UriKind.Relative)).Stream);
            _notifyIcon.Visible          = true;

            _notifyIcon.MouseClick       += new System.Windows.Forms.MouseEventHandler(notifyIcon_MouseClick);

            List<System.Windows.Forms.MenuItem> menuArray = new List<System.Windows.Forms.MenuItem>();
            //设置菜单项
            //System.Windows.Forms.MenuItem showmenu  = new System.Windows.Forms.MenuItem("显示", );
            //退出菜单项
            System.Windows.Forms.MenuItem exitMenu  = new System.Windows.Forms.MenuItem("退出", notifyIcon_exit);
            menuArray.Add(exitMenu);

            //关联托盘控件
            _notifyIcon.ContextMenu                      = new System.Windows.Forms.ContextMenu(menuArray.ToArray());
        }
        private void InitWndEvent()
        {
            log.Info("创建窗口控件事件响应");            
            //窗体按键事件处理
            this.Loaded                                         += MainWindow_Loaded;
            this.Closing                                        += MainWindow_Closing;

            InputScope      inPS                                = new InputScope();
            InputScopeName  inN                                 = new InputScopeName();
            inN.NameValue                                       = InputScopeNameValue.Default;
            inPS.Names.Add(inN);
            window.InputScope                                   = inPS;
            
            //AxSVCBoxMonitor相关事件响应           
            _mainWndowModel.boxControl.Start                    = _axSVCBoxMonitor.Start;
            _mainWndowModel.boxControl.LoadKeyMapping           = _axSVCBoxMonitor.LoadKeyMapping;
            _mainWndowModel.boxControl.SetKeyMappingOpen        = _axSVCBoxMonitor.SetKeyMappingOpen;
            _mainWndowModel.boxControl.SaveKeyMappingConfig     = _axSVCBoxMonitor.SaveKeyMappingConfig;
            _mainWndowModel.boxControl.SetRunningApp            = _axSVCBoxMonitor.SetRunningApp;
            _mainWndowModel.boxControl.OpenTaskManager          = _axSVCBoxMonitor.OpenTaskManager;
            _mainWndowModel.boxControl.CallAndroidBack          = _axSVCBoxMonitor.CallAndroidBack;
            _mainWndowModel.boxControl.CallAndroidHomeKey       = _axSVCBoxMonitor.CallAndroidHomeKey;
            _mainWndowModel.boxControl.Exit                     = _axSVCBoxMonitor.Exit;
            _mainWndowModel.boxControl.ClearTouchedKey          = _axSVCBoxMonitor.ClearTouchedKey;
            _mainWndowModel.boxControl.SetFlag                  = _axSVCBoxMonitor.SetFlag;
            _mainWndowModel.boxControl.UpdateVedioRect          = _axSVCBoxMonitor.UpdateVedioRect;
            _mainWndowModel.boxControl.UpdateDeviceSize         = _axSVCBoxMonitor.UpdateDeviceSize;
            _mainWndowModel.boxControl.UpdateCollsepRc          = _axSVCBoxMonitor.UpdateCollsepRc;
            _axSVCBoxMonitor.NotifyConnectStateChanged          += axSVCBoxMonitor_ConnectStateChanged;

            _mainWndowModel._keyMAPModel.LoadConfigAction       = _keyMap.LoadConfig;
            _mainWndowModel._keyMAPModel.SaveConfig             = _keyMap.SaveConfig;
            _mainWndowModel._keyMAPModel.getKeyMappingConfig    = _keyMap.getKeyMappingConfig;
        }

        private void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _notifyIcon.Dispose();
        }

        /*********************其它控件相关事件*****************************/
        /*拖盘图标退出选项*/
        private void notifyIcon_exit(object sender, EventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }
        /*拖盘图标鼠标单击*/
        private void notifyIcon_MouseClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                if (this.Visibility == Visibility.Visible)
                {
                    this.Visibility = Visibility.Hidden;
                }
                else
                {
                    this.Visibility = Visibility.Visible;
                    this.Activate();
                }
            }
        }
   
        /*********************MainWindow事件*****************************/
        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            //HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            //if (hwndSource != null)
            //{
            //    hwndSource.AddHook(new HwndSourceHook(this.WndProc));
            //}
        }

        protected virtual IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            try
            {
                switch (msg)
                {
                //case 0x400:
                //        //InvalidateVisual();
                //break;
                    case 0x0312://WM_HOTKEY
                        int lword = lParam.ToInt32() & 0xFFFF;
                        int hword = lParam.ToInt32() >> 16;

                        if (hword == Convert.ToUInt32(System.ConsoleKey.X)   &&
                        lword == Convert.ToUInt32(ModifierKeys.Control | ModifierKeys.Alt)
                            )
                        {
                            if (this.Visibility == Visibility.Visible)
                                this.Visibility = Visibility.Hidden;
                            else
                                this.Visibility = Visibility.Visible;
                        }

                        break;
                    case 0x0084://WM_NCHITTEST

                        if (_keyMap.IsVisible || window.WindowState == System.Windows.WindowState.Maximized)
                            break;

                        int     agWidth         = 6; //拐角宽度
                        int     bThickness      = 3; // 边框宽度

                        System.Windows.Point mousePt = new System.Windows.Point(lParam.ToInt32() &0xFFFF, lParam.ToInt32() >> 16);
                        mousePt = this.PointFromScreen(mousePt);

                        #region 测试鼠标位置

                        // 窗口左上角
                        if (mousePt.Y <= agWidth && 
                        mousePt.X <= agWidth)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTTOPLEFT);
                        }
                        // 窗口左下角    
                        else if (this.ActualHeight - mousePt.Y <= agWidth && 
                                mousePt.X <= agWidth)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTBOTTOMLEFT);
                        }
                        // 窗口右上角
                        else if (mousePt.Y <= agWidth && 
                                this.ActualWidth - mousePt.X <= agWidth)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTTOPRIGHT);
                        }
                        // 窗口右下角
                        else if (this.ActualWidth - mousePt.X <= agWidth && 
                                this.ActualHeight - mousePt.Y <= agWidth)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTBOTTOMRIGHT);
                        }
                        // 窗口左侧
                        else if (mousePt.X <= bThickness)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTLEFT);
                        }
                        // 窗口右侧
                        else if (this.ActualWidth - mousePt.X <= bThickness)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTRIGHT);
                        }
                        // 窗口上方
                        else if (mousePt.Y <= bThickness)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTTOP);
                        }
                        // 窗口下方
                        else if (this.ActualHeight - mousePt.Y <= bThickness)
                        {
                            handled = true;
                            return new IntPtr((int)Common.HitTest.HTBOTTOM);
                        }
                        //else  // 窗口移动
                        //{
                        //    HitTestResult result = VisualTreeHelper.HitTest(this, new System.Windows.Point(pt.X, pt.Y));

                        //    if(result == null || result.Equals(this))
                        //    {
                        //        handled = true;
                        //        return new IntPtr((int)Common.HitTest.HTCAPTION);
                        //    }
                        //}

                        #endregion
                        break;
                }
            }
            catch
            {

            }
          
            return IntPtr.Zero;
        }

        protected void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {

            IntPtr hwnd = new System.Windows.Interop.WindowInteropHelper(this).Handle;

            // 获得窗体的 样式
            int oldstyle = Common.Win32APIs.GetWindowLong(hwnd, Common.Win32APIs.GWL_STYLE);

            // 更改窗体的样式为无边框窗体
            Common.Win32APIs.SetWindowLong(hwnd, Common.Win32APIs.GWL_STYLE, oldstyle & ~Common.Win32APIs.WS_CAPTION);

            HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            Common.Win32APIs.RegisterHotKey(hwndSource.Handle, hwndSource.Handle.ToInt32(), Convert.ToUInt32(ModifierKeys.Control | ModifierKeys.Alt), Convert.ToUInt32(System.ConsoleKey.X));

            _waitWnd.Owner                          = this;
            _vedioListWnd.Owner                     = this;
            _keyMap.Owner                           = this;

            _waitWnd.Visibility                     = Visibility.Visible;
            _vedioListWnd.Visibility                = Visibility.Visible;
            _keyMap.Visibility                      = Visibility.Hidden;

            _waitWnd.Activate();
            _vedioListWnd.Activate();
            _keyMap.Activate();

            App.Current.MainWindow                  = this;
            App.Current.ShutdownMode                = ShutdownMode.OnMainWindowClose;
            //_axSVCBoxMonitor.Start(0, Convert.ToUInt32(hwndSource.Handle.ToInt32()), "127.0.0.1");
        }

        /*********************axSVCBoxMonitor控件反馈事件*****************************/
        private void axSVCBoxMonitor_ConnectStateChanged(object sender, AxSVCBoxMonitorLib._DSVCBoxMonitorEvents_NotifyConnectStateChangedEvent e)
        {
            try
            {
               _mainWndowModel.boxControl.NotifyOnConnectStateChanged(e.ulCurrentState == 1);
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
    }
}


