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

namespace UYouMain
{    
    [DataContract]
    public class Config{
        [DataMember]
        public string   runningApp = "com.blue.uyou.gamelauncher";
    }
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
        private System.Windows.Forms.NotifyIcon _notifyIcon         = null;
        private System.Windows.Point            _leftDowPoint       = new System.Windows.Point(0,0);
        private MainWindowContent               _mainWindowContent;
        private System.Timers.Timer             _voiceBtnTimer      = new System.Timers.Timer();
        private String                          _voiceCmd           = "add";
        private Boolean                         _bFullScreen        = false;
        private Config                          _config             = new Config();
        private WebRequest                      _webRequest         = null;
        private WebClient                       _webclient          = null;
        private string                          _configFile         = System.Windows.Forms.Application.StartupPath + "\\" + Common.Common.configFilePath + "\\"
                                                                    + Common.Common.configFile + Common.Common.configFileExt;
        
        private AxSVCBoxLib.AxSVCBox            _axSVCBox           = new AxSVCBoxLib.AxSVCBox();
        private KeyMapping                      _keyMap             = new KeyMapping();
        private waitWnd                         _waitWnd            = new waitWnd();
        private Upgrade                         _upgradeWnd         = null;
        private InstallAPP                      _installAPP         = null;
                    //_currentRunningApp = "com.blue.uyou.gamelauncher";

        private Boolean                         _bHandUpgrade       = false;        ////是否为手动更新
        private Boolean                         _bFirstUpgrade      = true;         ////是否为第一次更新
        private Emv_Oper_Upgrade_Data           _upgrade_Data;
        private bool                            _bConnected         = false;
        private bool                            _bFirst             = true;
        float                                   _dpiX               = 0;
        float                                   _dpiY               = 0;
        Rect                                    _vedioRect          = new Rect();

        public MainWindow()
        {
            InitializeComponent();
            InitialTray();

            using (Graphics graphics = Graphics.FromHwnd(IntPtr.Zero))
            {
                _dpiX = graphics.DpiX;
                _dpiY = graphics.DpiY;
            }
            //string lnkPath = System.Environment.GetFolderPath(Environment.SpecialFolder.Startup) + "\\" 
            //+ System.IO.Path.GetFileName(Assembly.GetExecutingAssembly().Location) +".lnk";
            //CreateLnk(lnkPath);

            //加载皮肤字典
            log.Info("加载皮肤字典");
            _mainWindowContent = System.Windows.Application.LoadComponent(new Uri("/UYouMain;component/Resources/skin/black.xaml", UriKind.Relative))
                as MainWindowContent;
            if (_mainWindowContent == null)
            {
                log.Error("皮肤字典加载失败");
                throw new Exception("皮肤字典加载失败");
            }


            if (_axSVCBox == null)
            {
                log.Error("SVCBox 播放插件加载失败");
                System.Windows.MessageBox.Show("播放插件加载失败");
                throw new Exception("SVCBox 播放插件加载失败");
            }

            System.Windows.Application.Current.Resources.MergedDictionaries.Add(_mainWindowContent);
            log.Info("资源字典加载完成");
            _mainWindowContent.host.Child       = _axSVCBox;

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
            this.Closing                                    += MainWindow_Closing;
            this.PreviewKeyDown                             += MainWindow_PreviewKeyDown;   
            this.SizeChanged                                += MainWindow_SizeChanged;    
            this.LocationChanged                            += MainWindow_LocationChanged;
            this.Loaded                                     += MainWindow_Loaded;
            this.MouseMove                                  += MainWindow_MouseMove;
            this.MouseDown                                  += MainWindow_MouseDown;
            this.Deactivated                                += MainWindow_Deactivated;
            this.Activated                                  += MainWindow_Activated;
            this.PreviewKeyUp                               += MainWindow_PreviewKeyUp;

            InputScope      inPS                            = new InputScope();
            InputScopeName  inN                             = new InputScopeName();
            inN.NameValue                                   = InputScopeNameValue.Default;
            inPS.Names.Add(inN);
            window.InputScope                               = inPS;
            _mainWindowContent.host.InputScope              = inPS;     

            
            //最大化等系统按钮
            _mainWindowContent.sliderVolume.ValueChanged    += sliderVolume_ValueChanged;
            _mainWindowContent.btnMax.Click                 += btnMax_Click;
            _mainWindowContent.btnExit.Click                += btnExit_Click;
            _mainWindowContent.btnMin.Click                 += btnMin_Click;

            _mainWindowContent.btnGameLauncher.Click        += btnGameLauncher_Click;
            _mainWindowContent.btnVideoLauncher.Click       += btnVideoLauncher_Click;

            _mainWindowContent.btnKeyMapping.Click          += btnKeyMappingLoad_Click;
            _keyMap.IsVisibleChanged                        += keymap_IsVisibleChanged;
            _keyMap.btnSave.Click                           += keyMappingbtnSave_Click;
            _mainWindowContent.chkKeyBoard.Click            += cbKeyBoard_Click;

            //Android按钮
            _mainWindowContent.btnHome.Click                += btnHome_Click;
            _mainWindowContent.btnBack.Click                += btnBack_Click;
            _mainWindowContent.btnTask.Click                += btnTask_Click;

            _mainWindowContent.btnMoreSetting.Click         += btnMoreSetting_Click;

            _mainWindowContent.cbMore.DropDownClosed        += cbMore_OnDropDownClosed;
            _mainWindowContent.cbMore.DropDownOpened        += cbMore_OnDropDownOpened;
            _mainWindowContent.cbMore.SelectionChanged      += cbMore_SelectionChanged;
            //AxSVCBox事件响应           
            _axSVCBox.NotifyDeviceScreenSize                += axSVCBox_NotifyDeviceScreenSize;
            _axSVCBox.NotifyRunningApp                      += axSVCBox_NotifyRunningApp;
            _axSVCBox.NotifySystemVolume                    += axSVCBox_NotifySystemVolume;
            _axSVCBox.ConnectStateChanged                   += axSVCBox_ConnectStateChanged;
            _axSVCBox.NotifySdCardSpace                     += axSVCBox_NotifySdCardSpace;
            _axSVCBox.NotifyUpgrade                         += axSVCBox_NotifyUpgrade;
            _axSVCBox.NotifyVideoReady                      += axSVCBox_NotifyVideoReady;
            _axSVCBox.NotifyUpdateException                 += axSVCBox_NotifyUpdateException;
            _axSVCBox.NotifyUpgrageFinish                   += axSVCBox_NotifyUpgrageFinish;
                        
             this.TextInput                                 += MainWindow_TextInput;

            _mainWindowContent.svcBoxGrid.SizeChanged       += svcBoxGrid_SizeChanged;

            _axSVCBox.AllowDrop = true;
            _mainWindowContent.CollapseButton.Checked       += collapseButton_Checked;
            _mainWindowContent.CollapseButton.Unchecked     += collapseButton_Unchecked;
            
            _voiceBtnTimer.Elapsed                          += new ElapsedEventHandler((s, e) => OnTimedEvent(s, e, this));
            
        }

        private void UpdateVedioRect()
        {
            if (this.IsLoaded)
            {   
                ////更新折叠按钮的屏幕区域 所有布局从外到内，因此应该取外部的宽高数据
                double leftmenuWidth                    = _mainWindowContent.CtrlGrid.ColumnDefinitions[0].ActualWidth;
                double titlePanelHeight                 = _mainWindowContent.rootPanel.RowDefinitions[0].ActualHeight;

                double boxGridWidth                     = ActualWidth - leftmenuWidth;
                double boxGridHeight                    = ActualHeight - titlePanelHeight;

                /*折叠按钮左顶点*/
                System.Windows.Point  ptTopLeftColla    = new System.Windows.Point(0, 0);
                /*折叠按钮右下点*/
                System.Windows.Point  ptBotRightColla   = new System.Windows.Point(0, 0);
                /*视频窗口左顶点*/
                System.Windows.Point  ptTopLeftVedio    = new System.Windows.Point(0, 0);
                /*视频窗口右下点*/
                System.Windows.Point  ptBotRightVedio   = new System.Windows.Point(0, 0);
                /*********************计算出相对于主窗口的坐标*******************/
                ptTopLeftColla.X                        = leftmenuWidth;
                ptTopLeftColla.Y                        = titlePanelHeight + (boxGridHeight - _mainWindowContent.CollapseButton.ActualHeight)/2;
                ptBotRightColla.X                       = leftmenuWidth + _mainWindowContent.CollapseButton.ActualWidth;
                ptBotRightColla.Y                       = titlePanelHeight + (boxGridHeight + _mainWindowContent.CollapseButton.ActualHeight)/2;

                ptTopLeftVedio.X                        = leftmenuWidth;
                ptTopLeftVedio.Y                        = titlePanelHeight;
                ptBotRightVedio.X                       = leftmenuWidth + boxGridWidth;
                ptBotRightVedio.Y                       = titlePanelHeight + boxGridHeight;
                /*************************************************************/


                /**********************转为屏幕像素坐标********************/
                ptTopLeftColla                          = PointToScreen(ptTopLeftColla);
                ptBotRightColla                         = PointToScreen(ptBotRightColla);
                _axSVCBox.UpdateCollsepRc(Convert.ToInt32(ptTopLeftColla.X), Convert.ToInt32(ptTopLeftColla.Y), Convert.ToInt32(ptBotRightColla.X), Convert.ToInt32(ptBotRightColla.Y));

                System.Windows.Point ptTopLeft          = PointToScreen(ptTopLeftVedio);
                System.Windows.Point ptBottomRight      = PointToScreen(ptBotRightVedio);
                _axSVCBox.UpdateVedioRect(Convert.ToInt32(ptTopLeft.X), Convert.ToInt32(ptTopLeft.Y), Convert.ToInt32(ptBottomRight.X), Convert.ToInt32(ptBottomRight.Y));
                _vedioRect = new Rect(ptTopLeft, ptBottomRight);

                /*********************转为屏幕WPF坐标*******************/
                if (WindowState == WindowState.Maximized)
                {

                }
                else
                {
                    ptTopLeftVedio.X                    += Left;
                    ptTopLeftVedio.Y                    += Top;
                    ptBotRightVedio.X                   += Left;
                    ptBotRightVedio.Y                   += Top;
                }
                /******************************************************/

                /*********************设置窗口屏幕WPF坐标*******************/
                _waitWnd.Left                           = ptTopLeftVedio.X;
                _waitWnd.Top                            = ptTopLeftVedio.Y;
                _waitWnd.Width                          = boxGridWidth;
                _waitWnd.Height                         = boxGridHeight;

                _keyMap.Left                            = ptTopLeftVedio.X;
                _keyMap.Top                             = ptTopLeftVedio.Y;
                _keyMap.Width                           = boxGridWidth;
                _keyMap.Height                          = boxGridHeight;
            }
        }

        private void LoadKeyMapping()
        {   //同步  映射数据和映射开关
            if (_config != null)
            {
                if (_config.runningApp != null && _config.runningApp != "")
                {
                    log.Info("LoadConfig" + _config.runningApp);

                    if (_keyMap.LoadConfig(_config.runningApp))
                    {
                        _axSVCBox.SetRunningApp(_config.runningApp);
                        _axSVCBox.LoadKeyMapping(_config.runningApp);
                        _axSVCBox.SetKeyMappingOpen(1);
                        _mainWindowContent.chkKeyBoard.IsChecked = true;
                        return;
                    }
                }
            }

            _axSVCBox.SetKeyMappingOpen(0);
            _keyMap.LoadConfig("");
            _mainWindowContent.chkKeyBoard.IsChecked = false;
        }

        private void SaveKeyMapping()
        {
            _keyMap.SaveConfig();
            KeyMappingConfig kmc = _keyMap.getKeyMappingConfig();
            saveKeyMappingConfig(kmc);
        }
        private void LoadConfig()
        {
            DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(Config));
            try
            {
                Directory.CreateDirectory(System.Windows.Forms.Application.StartupPath + "\\" + Common.Common.configFilePath);
                Directory.CreateDirectory(System.Windows.Forms.Application.StartupPath + "\\" + Common.Common.configFilePath + Common.Common.mappingConfigPath);
                using (FileStream stream = new FileStream(_configFile, FileMode.Open, FileAccess.Read, FileShare.Read))
                {
                    _config = (Config)serializer.ReadObject(stream);
                    stream.Close();
                }   
                window.Focus();
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void SaveConfig()
        {
            DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(Config));

            log.Info("save config info");
            Stream stream = new FileStream(_configFile, FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
            serializer.WriteObject(stream, _config);
            stream.Close();

        }
        private void MakeDeviceControlOpened()
        {
            if (IsLoaded)
            {
                if (_keyMap.Visibility == Visibility.Visible || _mainWindowContent.cbMore.IsDropDownOpen)
                    _axSVCBox.SetFlag(Convert.ToInt32(Common.BoxFlags.BoxFlags_IsOpenDeviceControl), 0);
                else
                    _axSVCBox.SetFlag(Convert.ToInt32(Common.BoxFlags.BoxFlags_IsOpenDeviceControl), 1);
            }
        }
        private void saveKeyMappingConfig(KeyMappingConfig kmc)
        {
            using (var mutex = new System.Threading.Mutex(false, "saveKeymappingtofile"))
            {
                if (_config.runningApp != "")
                {
                    try
                    {
                        string path = System.Windows.Forms.Application.StartupPath + Common.Common.configFilePath + Common.Common.mappingConfigPath;
                        System.IO.DirectoryInfo directoryinfo   = new System.IO.DirectoryInfo(path);
                        directoryinfo.Create();
                        path                                    += "\\" + _config.runningApp + Common.Common.configFileExt;
                        log.Info("saveKeyMappingConfig" + path);

                        List<Type> knowTypes                    = new List<Type>();
                        knowTypes.Add(typeof(Keys));
                        var serializer                          = new DataContractJsonSerializer(typeof(KeyMappingConfig), knowTypes);
                        Stream stream                           = new MemoryStream();
                        serializer.WriteObject(stream, kmc);
                        byte[] dataBytes                        = new byte[stream.Length];
                        stream.Position                         = 0;
                        stream.Read(dataBytes, 0, (int)stream.Length);
                        string dataString                       = Encoding.UTF8.GetString(dataBytes);
                        stream.Close();
                        if (kmc.Direction == null && kmc.Mapping.Count == 2 && kmc.Mapping[0] == null && kmc.Mapping[1] == null)
                        {   ////根据是否为空判断是不需要保存文件，如不需要则置为空，后面会先删除文件，并不会保存文件
                            dataString = "";
                        }

                        if (_axSVCBox.SaveKeyMappingConfig(path, dataString) == 1)
                        {
                            System.Windows.MessageBox.Show("键鼠映射保存成功");
                        }
                        else
                        {
                            System.Windows.MessageBox.Show("键鼠映射保存失败,请重新配置");
                        }
                    }
                    catch (Exception ex)
                    {
                        log.Error(ex.Message);
                    }
                }
            }
        }
        private void ExitFullScreen()
        {
            if (_bFullScreen)
            {
                _mainWindowContent.rootPanel.RowDefinitions[0].Height   = new System.Windows.GridLength(18);
                _mainWindowContent.CtrlGrid.ColumnDefinitions[0].Width  = new System.Windows.GridLength(1, GridUnitType.Star);
                _mainWindowContent.CollapseButton.IsChecked             = false;

                window.WindowState = System.Windows.WindowState.Normal;

                _bFullScreen = !_bFullScreen;
            }
        }
        private void SetCollapseMode(bool bCollapse)
        {
            if (bCollapse)
            {
                _mainWindowContent.CtrlGrid.ColumnDefinitions[0].Width  = new System.Windows.GridLength(0);
            }
            else
            {
                _mainWindowContent.CtrlGrid.ColumnDefinitions[0].Width  = new System.Windows.GridLength(1, GridUnitType.Star);
            }
            window.Focus();
        }
        private void SetFullScreenMode()
        {
            if (_bFullScreen == false)
            {
                _mainWindowContent.rootPanel.RowDefinitions[0].Height   = new System.Windows.GridLength(0);
                _mainWindowContent.CtrlGrid.ColumnDefinitions[0].Width  = new System.Windows.GridLength(0);
                _mainWindowContent.CollapseButton.IsChecked             = true;
                window.WindowState = System.Windows.WindowState.Maximized;
                _bFullScreen = !_bFullScreen;
            }
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
        private void collapseButton_Checked(object sender, RoutedEventArgs e)
        {
            SetCollapseMode(true);
        }
        private void collapseButton_Unchecked(object sender, RoutedEventArgs e)
        {
            SetCollapseMode(false);
        }
        private void cbMore_OnDropDownClosed(object sender, EventArgs e)
        {
            MakeDeviceControlOpened();
        }
        private void cbMore_OnDropDownOpened(object sender, EventArgs e)
        {
            MakeDeviceControlOpened();
        }
        private void btnMoreSetting_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_bConnected)
                {
                    _mainWindowContent.cbMore.IsDropDownOpen = true;
                }
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void cbMore_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            System.Windows.Controls.ComboBox cb = sender as System.Windows.Controls.ComboBox;
            switch (cb.SelectedIndex)
            {
                case 0: //安卓设置
                    {
                        String activityName = "com.android.settings.Settings";
                        String packageName  = "com.android.settings";
                        try
                        {
                            _axSVCBox.openApk(packageName, activityName);
                        }
                        catch (Exception ex)
                        {
                            log.Error(ex.Message);
                        }
                    }
                    break;
                case 3:
                    try
                    {
                        _axSVCBox.RebootBox();
                    }
                    catch (Exception ex)
                    {
                        log.Error(ex.Message);
                    }
                    break;
                case 4:
                    _bHandUpgrade = true;
                    _axSVCBox.GetUpgradeInfo(System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString());
                    break;
                case 5:
                    if (_bConnected == true)
                    {
                        if (_config.runningApp == "com.blue.uyou.gamelauncher")
                        {
                            if (_installAPP != null)
                            {
                                System.Windows.MessageBox.Show("一次只能安装一个APP！");
                                break;
                            }

                            Microsoft.Win32.OpenFileDialog ofd = new Microsoft.Win32.OpenFileDialog();
                            ofd.DefaultExt  = ".apk";
                            ofd.Filter      = "安卓应用文件|*.apk";
                            if (ofd.ShowDialog() == true)
                            {
                                window.Activate();

                                _installAPP         = new InstallAPP();
                                _installAPP.Owner   = this;
                                _installAPP.Topmost = true;
                                _installAPP.Show();
                                _installAPP.InstallBegin();

                                Thread t = new Thread(() =>
                                {
                                    Dispatcher.Invoke(new Action(() =>
                                    {
                                        try
                                        {
                                            _mainWindowContent.cbMore.IsEnabled = false;

                                            File.Copy(ofd.FileName, "1.apk", true);
                                            if (_axSVCBox.InstallAPP("1.apk") == 1)
                                            {
                                                //    System.Windows.MessageBox.Show("应用安装成功");
                                                _installAPP.InstallSuccess();
                                            }
                                            else
                                            {
                                                System.Windows.MessageBox.Show("应用安装失败");
                                                _installAPP.EndInstall();
                                            }
                                            File.Delete("1.apk");
                                            _mainWindowContent.cbMore.IsEnabled = true;
                                        }
                                        catch
                                        {
                                            _installAPP.EndInstall();
                                        }

                                        _installAPP = null;
                                    }));
                                });
                                t.Start();
                            }
                        }
                        else
                        {
                            System.Windows.MessageBox.Show("请先返回桌面首页");
                        }
                    }
                    else
                    {
                        System.Windows.MessageBox.Show("请先连接安卓设备");
                    }
                    break;
                case 6:
                    {

                        SetWnd _setWnd      = new SetWnd();
                        _setWnd.Owner       = this;
                        _setWnd.ShowDialog();
                    }
                    break;
            }
            cb.SelectedIndex = -1;
        }
        private void cbKeyBoard_Click(object sender, RoutedEventArgs e)
        {
            if ((sender as System.Windows.Controls.CheckBox).IsChecked == true)
            {
                _axSVCBox.SetKeyMappingOpen(1);
            }
            else
            {
                _axSVCBox.SetKeyMappingOpen(0);
            }
        }
        private void keymap_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            MakeDeviceControlOpened();
        }
        private void btnKeyMappingLoad_Click(object sender, RoutedEventArgs e)
        {
            LoadKeyMapping();
            _keyMap.Visibility = Visibility.Visible;
            _keyMap.Focus();
        }
        private void keyMappingbtnSave_Click(object sender, RoutedEventArgs e)
        {
            _keyMap.Visibility = Visibility.Hidden;
            SaveKeyMapping();
            LoadKeyMapping();

            this.Focus();
        }

        private void upgradeWnd_DownloadEnd(Upgrade sender, Upgrade.DownloadEventArgs e)
        {
            log.Info(e.UpgrageConfig.path);
            if (e.UpgradeScope == Upgrade.UpgradeScope.BOX)
            {
                _axSVCBox.PushUpgradeFileToBox(e.UpgrageConfig.path);
                _upgradeWnd._startUpgrade = Upgrade.UpgradeStatus.Done;
            }
            else if (e.UpgradeScope == Upgrade.UpgradeScope.PC)
            {                
                _axSVCBox.UpdatePCVersion(e.UpgrageConfig.path);
            }
        }        
        private void btnVideoLauncher_Click(object sender, RoutedEventArgs e)
        {                                  
            String activityName = "com.tcl.boxui.Launcher";
            String packageName  = "com.tcl.boxui";
            try
            {
                _axSVCBox.openApk(packageName, activityName);
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void btnGameLauncher_Click(object sender, RoutedEventArgs e)
        {
            
            String activityName = "com.blue.uyou.gamelauncher.Launcher";
            String packageName  = "com.blue.uyou.gamelauncher";
            try
            {
                _axSVCBox.openApk(packageName, activityName);
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
            
        }   

        /// <summary>
        /// 长按音量加减按键，控制音量加减
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <param name="mainWnd"></param>
        /// <returns></returns>
        private void OnTimedEvent(object sender, System.Timers.ElapsedEventArgs e, MainWindow  mainWnd)
        {
            switch (mainWnd._voiceCmd)
            {
                case "add":
                    _mainWindowContent.addVol();
                    /*
                    this.mainWindowEvent.SliderVoice.Dispatcher.Invoke(
                        new Action(delegate{
                            if (this.mainWindowEvent.SliderVoice.Value <= 98)
                            {
                                this.mainWindowEvent.SliderVoice.Value += 2;
                            }
                            else
                            {
                                this.mainWindowEvent.SliderVoice.Value = 100;
                            }
                        }));
                    */
                    break;
                case "sub":
                    _mainWindowContent.subVol();
                    /*
                    this.mainWindowEvent.SliderVoice.Dispatcher.Invoke(
                        new Action(delegate{
                            if (this.mainWindowEvent.SliderVoice.Value >= 2)
                            {
                                this.mainWindowEvent.SliderVoice.Value -= 2;
                            }
                            else
                            {
                                this.mainWindowEvent.SliderVoice.Value = 0;
                            }
                        }));
                    
                     */
                    break;
                default :
                    break;
            }
        }
        private void btnSlide_Click(object sender, RoutedEventArgs e)
        {            
        }
        private void btnTask_Click(object sender, RoutedEventArgs e)
        {            
            try
            {                
                _axSVCBox.OpenTaskManager();                         
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }

        }
        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            try
            {                
                _axSVCBox.CallAndroidBack();
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void btnHome_Click(object sender, RoutedEventArgs e)
        {
            _axSVCBox.CallAndroidHomeKey();            
        }
        private void sliderVolume_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (_bFirst == false)
            {
                int value = (int)e.NewValue;               
                _axSVCBox.SetVolume(value);
            }
            _bFirst = false;
        }
        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btnMax_Click(object sender, RoutedEventArgs e)
        {
            if (_bConnected == false)
            {
                return;
            }
            _axSVCBox.ClearTouchedKey();
            //_axSVCBox.SetScreenResolution("720p");
            SetFullScreenMode();
        }
        private void btnMin_Click(object sender, RoutedEventArgs e)
        {
            _axSVCBox.ClearTouchedKey();
            window.WindowState = System.Windows.WindowState.Minimized;
        }
        private void svcBoxGrid_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            UpdateVedioRect();

            Thread t = new Thread(() =>
            {
                Dispatcher.Invoke(new Action(() =>
                {
                    try
                    {
                        Thread.Sleep(50);
                        _mainWindowContent.leftMenu.InvalidateVisual();
                    }
                    catch
                    {

                    }
                }));
            });
            t.Start();
        }
        /*********************MainWindow事件*****************************/
        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);
            HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            if (hwndSource != null)
            {
                hwndSource.AddHook(new HwndSourceHook(this.WndProc));
            }
        }
        protected override void OnRender(DrawingContext drawingContext)
        {
            base.OnRender(drawingContext);
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
        protected void MainWindow_LocationChanged(object sender, EventArgs e)
        {
            UpdateVedioRect();
        }
        protected void MainWindow_TextInput(object sender, TextCompositionEventArgs e)
        {

            bool b = IsFocused;
            //log.Info("TextInput" + e.Text + "\n");            
            char[] value = e.Text.ToCharArray();
            //if (!fullScreen)
            {
                foreach (char c in value)
                {
                    _axSVCBox.InputChar(c);
                }
            }
        }
        protected void MainWindow_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            System.Windows.Point pt     = e.GetPosition(this);
            System.Windows.Point pt0    = e.GetPosition(_mainWindowContent.svcBoxGrid);
            System.Windows.Point pt1    = e.GetPosition(_mainWindowContent.title);//在标题栏的区域内或左边
            System.Windows.Point pt2    = e.GetPosition(_mainWindowContent.txtDiskSpace);//在标题栏操控区的左边
            if (e.LeftButton == MouseButtonState.Pressed                        && 
            Math.Abs(pt.X - _leftDowPoint.X) < 5                                &&
            Math.Abs(pt.Y - _leftDowPoint.Y) < 5                                &&
            (pt0.X <= 0 || pt0.Y <= 0)                                          && 
            (pt1.X <= _mainWindowContent.title.ActualWidth || pt2.X <= 0)
            )
            {//如果在标题栏按下鼠标左键，且移动，则触发窗体移动
                this.DragMove();
            }

            _leftDowPoint = new System.Windows.Point(0,0);
        }

        private void MainWindow_MouseDown(object sender, MouseButtonEventArgs e)
        {
            _leftDowPoint = e.GetPosition(this);
        }
        protected void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //清除热键
            HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            if (hwndSource != null)
                Common.Win32APIs.UnregisterHotKey(hwndSource.Handle, hwndSource.Handle.ToInt32());

            try
            {
                ////先清除弹出窗口
                if (_upgradeWnd != null)
                {
                    _upgradeWnd.Close();
                    _upgradeWnd = null;
                }
                if (_installAPP != null)
                {
                    _installAPP.Close();
                    _installAPP = null;
                }
                _notifyIcon.Dispose();

                _axSVCBox.ClearTouchedKey();
                _axSVCBox.Exit();                    ////控件优先退出

                SaveConfig();
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }

        protected void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
#if  _TEST_VERSION
#else
   /************************这里连接导向百度统计的网页，进行数据统计***************************/
            _mainWindowContent.webBrowser.Visibility = Visibility.Hidden;
            _mainWindowContent.webBrowser.Navigate(@"http://stat.52uyou.com");

            int sleepTime = 10 * 60 * 1000;
            Thread t1 = new Thread(() =>
            {
                while (true)
                {
                    Thread.Sleep(sleepTime);
                    try
                    {
                        Dispatcher.Invoke(new Action(() =>
                        {
                            _mainWindowContent.webBrowser.Refresh();
                        }
                        ));
                    }
                    catch (Exception ex)
                    {
                        string str  = ex.Message;
                    }
                }
            });
            t1.IsBackground = true;
            t1.Start();
            /***********************************************************************************/
#endif

            //获取系统音量，并显示
            /*
            mainWindowEvent.SliderVoice.Maximum = 100;
            mainWindowEvent.SliderVoice.Minimum = 0;
            mainWindowEvent.SliderVoice.Value = 10;                   
            */

            IntPtr hwnd = new System.Windows.Interop.WindowInteropHelper(this).Handle;

            // 获得窗体的 样式
            int oldstyle = Common.Win32APIs.GetWindowLong(hwnd, Common.Win32APIs.GWL_STYLE);

            // 更改窗体的样式为无边框窗体
            Common.Win32APIs.SetWindowLong(hwnd, Common.Win32APIs.GWL_STYLE, oldstyle & ~Common.Win32APIs.WS_CAPTION);

            int Major       = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Major;
            int Minor       = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Minor;
            int Build       = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Build;
            int Revision    = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Revision;


            HwndSource hwndSource = PresentationSource.FromVisual(this) as HwndSource;
            Common.Win32APIs.RegisterHotKey(hwndSource.Handle, hwndSource.Handle.ToInt32(), Convert.ToUInt32(ModifierKeys.Control | ModifierKeys.Alt), Convert.ToUInt32(System.ConsoleKey.X));

            Thread t = new Thread(() =>
            {
                Dispatcher.Invoke(new Action(() =>
                {
                    try
                    {
                        _axSVCBox.Start(1, Convert.ToUInt32(hwndSource.Handle.ToInt32()));
                        //_axSVCBox.Start(1, Convert.ToUInt32(hwnd.ToInt32()));
                    }
                    catch
                    {

                    }
                }));
            });
            t.Start();


#if _TEST_VERSION
            _mainWindowContent.title.Text = "U游安卓世界" + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString();
#else
            _mainWindowContent.title.Text = "U游安卓世界" + Major + "." + Minor + "." + Build;
#endif

            _mainWindowContent.host.Focus();
            _mainWindowContent.host.Visibility      = Visibility.Hidden;

            _waitWnd.Owner                          = this;
            _waitWnd.Visibility                     = System.Windows.Visibility.Visible;
            _keyMap.Owner                           = this;
            _keyMap.Visibility                      = Visibility.Hidden;
            UpdateVedioRect();
        }
        protected void MainWindow_Deactivated(object sender, EventArgs e)
        {
            var window = sender as Window;
            window.Topmost = false;
            if (_installAPP != null)
            {
                _installAPP.Topmost = false;
            }
            _axSVCBox.ClearTouchedKey();
        }
        protected void MainWindow_PreviewKeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
            {//ESC按键响应Android返回事件       
                ExitFullScreen();
            }
            else if (e.Key == Key.Left       ||
                        e.Key == Key.Up         ||
                        e.Key == Key.Right      ||
                        e.Key == Key.Down
                    )
            {
                e.Handled = true;
            }
        }
        protected void MainWindow_Activated(object sender, EventArgs e)
        {
            var window = sender as Window;
            //window.Topmost = true;
            _axSVCBox.ClearTouchedKey();
        }
        protected void MainWindow_SizeChanged(object sender, SizeChangedEventArgs e)
        {
        }
        protected void MainWindow_PreviewKeyUp(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.Tab)
            {
                window.Focus();
            }
        }
        /*********************axSVCBox控件反馈事件*****************************/
        private void axSVCBox_NotifyUpgrade(object sender, AxSVCBoxLib._DSVCBoxEvents_NotifyUpgradeEvent e)
        {
            try
            {
                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Emv_Oper_Upgrade_Data));
                Byte[] bytes = Encoding.UTF8.GetBytes(e.upgradeInfo);
                log.Info("axSVCBox_NotifyUpgrade" + e.upgradeInfo);
                MemoryStream stream = new MemoryStream(bytes);
                _upgrade_Data = (Emv_Oper_Upgrade_Data)ser.ReadObject(stream);
                if (_upgrade_Data != null)
                {
                    bool bShowUpdate = false;
                    if (_upgrade_Data.box != null               && 
                        _upgrade_Data.box.downloadurl != null   && 
                        _upgrade_Data.box.downloadurl != ""
                        )
                    {
                        bShowUpdate = true;
                    }
                    else if (_upgrade_Data.pc != null                && 
                                _upgrade_Data.pc.downloadurl != null    && 
                                _upgrade_Data.pc.downloadurl != ""
                            )
                    {
                        string curVersion = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString();
                        if (_upgrade_Data.pc.vervalue.CompareTo(curVersion) > 0)    /////如果更新版本大于当前版本才提示
                            bShowUpdate = true;
                    }
                    else
                    {
                        if (_bHandUpgrade)    ////如果手动更新时才显示最新版本提示
                            System.Windows.MessageBox.Show("您使用的已经是最新版本");
                    }

                    if (bShowUpdate)
                    {   ////如果不是第一次更新 又不是手动更新，就不提示更新提示框
                        if (_bHandUpgrade == false && _bFirstUpgrade == false)
                            bShowUpdate = false;
                    }

                    if (bShowUpdate == true)
                    {
                        if (_upgradeWnd == null)
                        {
                            _upgradeWnd = new Upgrade();
                            _upgradeWnd.DownloadEnd += upgradeWnd_DownloadEnd;
                        }
                        else
                        {
                            if (_upgradeWnd.Visibility == Visibility.Visible)
                            {
                                _upgradeWnd.Hide();
                                return;
                            }
                        }

                        if (_upgradeWnd._startUpgrade == Upgrade.UpgradeStatus.NONE || _upgradeWnd._startUpgrade == Upgrade.UpgradeStatus.Done)
                        {
                            _upgradeWnd.Initialize(_upgrade_Data);

                        }
                        _upgradeWnd.Show();
                    }
                }
                /*
                DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Emv_Oper_Upgrade_Data));
                Byte[] bytes = Encoding.UTF8.GetBytes(e.upgradeInfo);
                MemoryStream stream = new MemoryStream(bytes);
                Emv_Oper_Upgrade_Data data = (Emv_Oper_Upgrade_Data)ser.ReadObject(stream);
                String message = "";
                if (data.box.vervalue != null)
                {
                    message = "检测到新的Android版本 " + data.box.vervalue;
                }
                if (data.pc.vervalue != null)
                {
                    message += "检测到新的PC版本 " + data.box.vervalue;
                }
                message += "，请确认是否升级";
                if (System.Windows.Forms.MessageBox.Show(message, "系统提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == System.Windows.Forms.DialogResult.OK)
                {
                    axSVCBox.UpdateSystem(e.upgradeInfo);
                }
                */
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }

            _bHandUpgrade    = false;
            _bFirstUpgrade   = false;
        }
        private void axSVCBox_NotifyUpgrageFinish(object sender, EventArgs e)
        {
            {
                log.Info("axSVCBox_NotifyUpgrageFinish\n");
                _upgradeWnd._startUpgrade = Upgrade.UpgradeStatus.Done;

                _upgradeWnd.Hide();
            }
        }
        private void axSVCBox_NotifyUpdateException(object sender, EventArgs e)
        {
            log.Error("axSVCBox_NotifyUpdateException\n");
            if(_upgradeWnd != null)
            {
                _upgradeWnd._startUpgrade   = Upgrade.UpgradeStatus.Done;
                _upgradeWnd.title.Text      = "升级失败，请重试";
            }
        }
        private void axSVCBox_NotifyDeviceScreenSize(object sender, AxSVCBoxLib._DSVCBoxEvents_NotifyDeviceScreenSizeEvent e)
        {
            try
            {
                String s1 = e.ulWidth.ToString();
                String s2 = e.ulHeight.ToString();

                log.Info("axSVCBox_NotifyDeviceScreenSize" + s1 + s2);
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void axSVCBox_NotifyVideoReady(object sender, EventArgs e)
        {
        }
        private void axSVCBox_NotifySdCardSpace(object sender, AxSVCBoxLib._DSVCBoxEvents_NotifySdCardSpaceEvent e)
        {
            try
            {
                double value                            = Convert.ToDouble(e.ulUsed) / Convert.ToDouble(e.ulTotal);
                _mainWindowContent.txtDiskSpace.Text    = "剩余空间" + value.ToString("P2");
                _mainWindowContent.progSpace.Maximum    = e.ulTotal;
                _mainWindowContent.progSpace.Value      = e.ulTotal - e.ulUsed;

                _waitWnd.Visibility                     = System.Windows.Visibility.Hidden;
                _bConnected                             = true;
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void axSVCBox_ConnectStateChanged(object sender, AxSVCBoxLib._DSVCBoxEvents_ConnectStateChangedEvent e)
        {
            try
            {
                if (e.ulCurrentState == 1)
                {
                    int volume = 0;
                    _axSVCBox.GetDeviceScreenSize();    //这个不准确;
                    _axSVCBox.UpdateDeviceSize(Common.Common.boxSize.Width, Common.Common.boxSize.Height); //盒子内部分辨率
                    _axSVCBox.GetSDCardSpace();
                    _axSVCBox.GetUpgradeInfo(System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString());
                    _axSVCBox.GetAndroidVolume(ref volume);

                    _waitWnd.Visibility = System.Windows.Visibility.Hidden;

                    LoadConfig();
                    LoadKeyMapping();
                    UpdateVedioRect();

                    _bConnected = true;
                }
                else
                {
                    _bConnected = false;
                    //重连

                    ExitFullScreen();
                    _mainWindowContent.txtDiskSpace.Text = "连接已断开";
                    _mainWindowContent.progSpace.Value = 0;


                    window.Activate();
                    _waitWnd.Visibility = System.Windows.Visibility.Visible;
                }
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }
        private void axSVCBox_NotifySystemVolume(object sender, AxSVCBoxLib._DSVCBoxEvents_NotifySystemVolumeEvent e)
        {
            log.Info(e.ToString());
            _mainWindowContent.sliderVolume.Value = e.volume;
        }
        private void axSVCBox_NotifyRunningApp(object sender, AxSVCBoxLib._DSVCBoxEvents_NotifyRunningAppEvent e)
        {
            try
            {
                log.Info("axSVCBox_NotifyRunningApp" + e.bstrPackageName);

                _config.runningApp = e.bstrPackageName;
                LoadKeyMapping();
            }
            catch (Exception ex)
            {
                log.Error(ex.Message);
            }
        }

        private void CommandBinding_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            if(_bConnected)
            {
                //log.Info("TextInput" + e.Text + "\n");            
                String value = Clipboard.GetText();
                int count = 0;
                foreach (char c in value)
                {
                    if(count >= 500)
                        break;
                    _axSVCBox.InputChar(c);
                    count++;
                }
            }
        }

        private void CommandBinding_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = Clipboard.ContainsText();
        }
    }
}


