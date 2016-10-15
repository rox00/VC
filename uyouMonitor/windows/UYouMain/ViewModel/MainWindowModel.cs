using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Windows;
using System.Windows.Input;
using UYouMain.Common;

namespace UYouMain.ViewModel
{
    class BoxControl
    {
        public Func<uint, uint, string, uint>       Start;
        public Func<string, uint>                   LoadKeyMapping;
        public Func<short, uint>                    SetKeyMappingOpen;
        public Func<string, string, uint>           SaveKeyMappingConfig;
        public Func<string, uint>                   SetRunningApp;
        public Func<uint>                           OpenTaskManager;
        public Func<uint>                           CallAndroidBack;
        public Func<uint>                           CallAndroidHomeKey;
        public Func<uint>                           Exit;
        public Action                               ClearTouchedKey;
        public Action<int,int>                      SetFlag;
        public Action<int,int,int,int>              UpdateVedioRect;
        public Action<int,int>                      UpdateDeviceSize;
        public Action<int,int,int,int>              UpdateCollsepRc;


        public Action<bool>                         NotifyOnConnectStateChanged;
    }

    [DataContract]
    public class Config
    {
        [DataMember]
        public string   runningApp = "com.blue.uyou.gamelauncher";
    }

    class MainWindowModel : NotificationObject
    {
        #region 成员
        private Config                          _config         = new Config();
        public BoxControl                       boxControl      = new BoxControl();
        private string                          _configFile     = System.Windows.Forms.Application.StartupPath + "\\" + Common.Common.configFilePath + "\\"
                                                                        + Common.Common.configFile + Common.Common.configFileExt;

        public VedioListModel                   _vedioListModel { get; set; }
        public WaitWndModel                     _waitWndModel { get; set; }
        public KeyMapModel                      _keyMAPModel { get; set; }


        private WindowState wndState;

        public WindowState WndState
        {
            get { return wndState; }
            set
            {
                wndState = value;
                this.RaisePropertyChanged("WndState");
            }
        }

        /****************************************************/
        private bool                          _isCollapsed;
        public bool _IsCollapsed
        {
            get { return _isCollapsed; }
            set
            {
                _isCollapsed = value;
                this.RaisePropertyChanged("_IsCollapsed");

                if(_isCollapsed)
                    _LEFTMENUWIDTH = 0;
                else
                    _LEFTMENUWIDTH = 60;
            }
        }
        /****************************************************/
        
        /****************************************************/
        private double                          _leftmenuwidth;
        public double _LEFTMENUWIDTH
        {
            get { return _leftmenuwidth; }
            set
            {
                _leftmenuwidth = value;
                this.RaisePropertyChanged("_LEFTMENUWIDTH");
            }
        }
        /****************************************************/

        /****************************************************/
        private double                          _titleheight;
        public double _TitleHeight
        {
            get { return _titleheight; }
            set
            {
                _titleheight = value;
                this.RaisePropertyChanged("_TitleHeight");
            }
        }
        /****************************************************/

        /****************************************************/
        private string                          title;
        public string Title
        {
            get { return title; }
            set
            {
                title = value;
                this.RaisePropertyChanged("Title");
            }
        }
        /****************************************************/
        /****************************************************/
        private string                          conStatuText;
        public string ConStatuText
        {
            get { return conStatuText; }
            set
            {
                conStatuText = value;
                this.RaisePropertyChanged("ConStatuText");
            }
        }
        /****************************************************/

        /****************************************************/
        private bool                          isConnect;
        public bool IsConnect
        {
            get { return isConnect; }
            set
            {
                isConnect = value;
                this.RaisePropertyChanged("IsConnect");
            }
        }
        /****************************************************/

        /****************************************************/
        private double                          collapseWidth;
        public double CollapseWidth
        {
            get { return collapseWidth; }
            set
            {
                collapseWidth = value;
                this.RaisePropertyChanged("CollapseWidth");
            }
        }
        /****************************************************/
        /****************************************************/
        private double                          collapseHeight;
        public double CollapseHeight
        {
            get { return collapseHeight; }
            set
            {
                collapseHeight = value;
                this.RaisePropertyChanged("CollapseHeight");
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

        /****************************************************/
        private bool isHorizontalMode;
        public bool IsHorizontalMode
        {
            get { return isHorizontalMode; }
            set
            {
                isHorizontalMode = value;
                this.RaisePropertyChanged("IsHorizontalMode");
            }
        }
        /****************************************************/
        /****************************************************/
        private bool isKeyMappingOpen;
        public bool IsKeyMappingOpen
        {
            get { return isKeyMappingOpen; }
            set
            {
                isKeyMappingOpen = value;
                this.RaisePropertyChanged("IsKeyMappingOpen");
                boxControl.SetKeyMappingOpen(isKeyMappingOpen?(short)1:(short)0);
            }
        }
        /****************************************************/
        #endregion


        public DelegateCommand<object>              rotateCommand { get; set; }
        public DelegateCommand<object>              LoadCommand { get; set; }
        public DelegateCommand<object>              ExitCommand { get; set; }
        public DelegateCommand<object>              MaxCommand { get; set; }
        public DelegateCommand<object>              NormalCommand { get; set; }
        public DelegateCommand<object>              MinCommand { get; set; }
        public DelegateCommand<object>              UpdateWndPosCommand { get; set; }
        public DelegateCommand<object>              ActiviListWndCommand { get; set; }
        public DelegateCommand<object>              ActiviKeyMapWndCommand { get; set; }
        public DelegateCommand<object>              SynClipboardCommand { get; set; }
        public DelegateCommand<object>              DragWndCommand { get; set; }
        public DelegateCommand<object>              SaveKeyMappingCommand { get; set; }
        public DelegateCommand<object>              TaskCommand { get; set; }
        public DelegateCommand<object>              HomeCommand { get; set; }
        public DelegateCommand<object>              BackCommand { get; set; }
        public DelegateCommand<object>              DeviceControlCommand { get; set; }
        public DelegateCommand<object>              ScreenShotCommand { get; set; }
        public MainWindowModel()
        {
            _vedioListModel                         = new VedioListModel();
            _waitWndModel                           = new WaitWndModel();
            _keyMAPModel                            = new KeyMapModel();
            isHorizontalMode                        = false;
            wndState                                = WindowState.Normal;
            title                                   = ShowTitleVersion("");
            conStatuText                            = "已断开连接";
            windowLeft                              = 0;
            windowTop                               = 0;
            windowWidth                             = 0;
            windowHeight                            = 0;

            _isCollapsed                            = false;
            _leftmenuwidth                          = 60;
            _titleheight                            = 18;
            collapseWidth                           = 12;
            collapseHeight                          = 70;
            isKeyMappingOpen                        = false;

            using (Graphics graphics = Graphics.FromHwnd(IntPtr.Zero))
            {
                if (isHorizontalMode)
                {
                    if (graphics.DpiX > 0)
                        WindowWidth                 = 1280 * 96 / graphics.DpiX + _LEFTMENUWIDTH;
                    if (graphics.DpiY > 0)
                        WindowHeight                = 720 * 96 / graphics.DpiY  + 18;

                }
                else
                {
                    double  _MAX_ANDRIOD_HEIGHT     = (System.Windows.SystemParameters.PrimaryScreenHeight - 100 - _TitleHeight);
                    double  _WIDTH_HEIGHT_RATIO     = 9 / 16.0;

                    WindowWidth                     = _MAX_ANDRIOD_HEIGHT * 1.0 * _WIDTH_HEIGHT_RATIO + _LEFTMENUWIDTH;
                    WindowHeight                    = _MAX_ANDRIOD_HEIGHT * 1.0;
                }
            }


            boxControl.NotifyOnConnectStateChanged  = OnNotifyOnConnectStateChanged;
            LoadCommand                             = new DelegateCommand<object>(new Action<object>(this.ViewLoaded));
            ExitCommand                             = new DelegateCommand<object>(new Action<object>(this.ExitWnd));
            MaxCommand                              = new DelegateCommand<object>(new Action<object>(this.MaxWnd));
            NormalCommand                           = new DelegateCommand<object>(new Action<object>(this.NormalWnd));
            MinCommand                              = new DelegateCommand<object>(new Action<object>(this.MinWnd));
            rotateCommand                           = new DelegateCommand<object>(new Action<object>(this.Rotate));
            UpdateWndPosCommand                     = new DelegateCommand<object>(new Action<object>(this.UpdateWndPos));
            ActiviListWndCommand                    = new DelegateCommand<object>(new Action<object>(this.ActivitListWnd));
            ActiviKeyMapWndCommand                  = new DelegateCommand<object>(new Action<object>(this.ActivitKeyMapWnd));
            SynClipboardCommand                     = new DelegateCommand<object>(new Action<object>(this.SynClipboard));
            DragWndCommand                          = new DelegateCommand<object>(new Action<object>(this.DragWnd));
            SaveKeyMappingCommand                   = new DelegateCommand<object>(new Action<object>(this.SaveKeyMapping));
            TaskCommand                             = new DelegateCommand<object>(new Action<object>(this.Task));
            HomeCommand                             = new DelegateCommand<object>(new Action<object>(this.Home));
            BackCommand                             = new DelegateCommand<object>(new Action<object>(this.Back));
            DeviceControlCommand                    = new DelegateCommand<object>(new Action<object>(this.DeviceControlOpenClose));
            ScreenShotCommand                       = new DelegateCommand<object>(new Action<object>(this.ScreenShot));
            
        }


        #region 处理方法

        protected void ViewLoaded(object obj)
        {
            _vedioListModel.Start = boxControl.Start;
            UpdateWndPos(obj);
        }
        private void Task(object obj)
        {
            boxControl.OpenTaskManager();                         
        }
        private void Home(object obj)
        {
            boxControl.CallAndroidHomeKey();
        }
        private void Back(object obj)
        {
            boxControl.CallAndroidBack();
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
                
            }
            catch (Exception ex)
            {
                Debug.WriteLine(GetType() + ":" + ex.Message);
            }
        }
        private void SaveConfig()
        {
            try
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(Config));

                Debug.WriteLine(GetType() + ":" + "save config info");
                Stream stream = new FileStream(_configFile, FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
                serializer.WriteObject(stream, _config);
                stream.Close();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(GetType() + ":" + ex.Message);
            }
        }
        private void LoadKeyMapping()
        {   //同步  映射数据和映射开关
            if (_config != null)
            {
                if (_config.runningApp != null && _config.runningApp != "")
                {
                    if (_keyMAPModel.LoadConfig(_config.runningApp))
                    {
                        boxControl.SetRunningApp(_config.runningApp);
                        boxControl.LoadKeyMapping(_config.runningApp);
                        IsKeyMappingOpen = true;
                        return;
                    }
                }
            }

            _keyMAPModel.LoadConfigAction("");
            IsKeyMappingOpen = false;
        }

        private void SaveKeyMapping(object obj)
        {
            _keyMAPModel.WndVisible = Visibility.Hidden;

            _keyMAPModel.SaveConfig();
            KeyMappingConfig kmc = _keyMAPModel.getKeyMappingConfig();
            saveKeyMappingConfig(kmc);
            LoadKeyMapping();
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
                        Debug.WriteLine(GetType() + ":" + "saveKeyMappingConfig" + path);

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

                        if (boxControl.SaveKeyMappingConfig(path, dataString) == 1)
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
                        Debug.WriteLine(GetType() + ":" + ex.Message);
                    }
                }
            }
        }

        private void ActivitListWnd(object obj)
        {
            _vedioListModel.WndVisible = (_vedioListModel.WndVisible == Visibility.Visible)?Visibility.Hidden:Visibility.Visible;
        }

        private void ActivitKeyMapWnd(object obj)
        {
            _keyMAPModel.WndVisible = Visibility.Visible;
            LoadKeyMapping();
        }
        private void DeviceControlOpenClose(object obj)
        {
            boxControl.SetFlag(Convert.ToInt32(Common.BoxFlags.BoxFlags_IsOpenDeviceControl), _vedioListModel.WndVisible == Visibility.Hidden?1:0);
        }

        private void ScreenShot(object obj)
        {
            boxControl.SetFlag(Convert.ToInt32(Common.BoxFlags.BoxFlags_ScreenShot), 1);
        }

        private void SynClipboard(object obj)
        {
            if (IsConnect)
            {
                boxControl.ClearTouchedKey();

                String value = Clipboard.GetText();
                _vedioListModel.PasteStr(value);
            }
        }
        private void UpdateWndPos(object obj)
        {
            Window wnd = obj as Window;
            ////三个坐标： Visual下  窗口内坐标，窗口外坐标， 屏幕中的像素坐标
            ////更新折叠按钮的屏幕区域 所有布局从外到内，因此应该取外部的宽高数据
            double leftmenuWidth                        = _LEFTMENUWIDTH;
            double titlePanelHeight                     = _TitleHeight;

            double boxGridWidth                         = WindowWidth - leftmenuWidth;
            double boxGridHeight                        = windowHeight - titlePanelHeight;

            /*折叠按钮左顶点*/
            System.Windows.Point  ptTopLeftColla        = new System.Windows.Point(0, 0);
            /*折叠按钮右下点*/
            System.Windows.Point  ptBotRightColla       = new System.Windows.Point(0, 0);
            /*视频窗口左顶点*/
            System.Windows.Point  ptTopLeftVedio        = new System.Windows.Point(0, 0);
            /*视频窗口右下点*/
            System.Windows.Point  ptBotRightVedio       = new System.Windows.Point(0, 0);
            /*********************计算出相对于主窗口的坐标*******************/
            ptTopLeftColla.X                            = leftmenuWidth;
            ptTopLeftColla.Y                            = titlePanelHeight + (boxGridHeight - CollapseHeight)/2;
            ptBotRightColla.X                           = leftmenuWidth + CollapseWidth;
            ptBotRightColla.Y                           = titlePanelHeight + (boxGridHeight + CollapseHeight)/2;

            ptTopLeftVedio.X                            = leftmenuWidth;
            ptTopLeftVedio.Y                            = titlePanelHeight;
            ptBotRightVedio.X                           = leftmenuWidth + boxGridWidth;
            ptBotRightVedio.Y                           = titlePanelHeight + boxGridHeight;
            /*************************************************************/


            /**********************转为屏幕像素坐标********************/
            ptTopLeftColla                              = wnd.PointToScreen(ptTopLeftColla);
            ptBotRightColla                             = wnd.PointToScreen(ptBotRightColla);
            boxControl.UpdateCollsepRc(Convert.ToInt32(ptTopLeftColla.X), Convert.ToInt32(ptTopLeftColla.Y), Convert.ToInt32(ptBotRightColla.X), Convert.ToInt32(ptBotRightColla.Y));

            System.Windows.Point ptTopLeftVedioScr      = wnd.PointToScreen(ptTopLeftVedio);
            System.Windows.Point ptBottomRightVedioScr  = wnd.PointToScreen(ptBotRightVedio);

            if (IsHorizontalMode == false && WndState == WindowState.Maximized)
            {
                double wndWidth                         = (ptBottomRightVedioScr.X - ptTopLeftVedioScr.X);
                double vediowidth                       = (ptBottomRightVedioScr.Y - ptTopLeftVedioScr.Y)/16 * 9;
                ptTopLeftVedioScr.X                     = (wndWidth - vediowidth)/2;
                ptBottomRightVedioScr.X                 = (wndWidth + vediowidth)/2;

                boxControl.UpdateVedioRect(Convert.ToInt32(ptTopLeftVedioScr.X), Convert.ToInt32(ptTopLeftVedioScr.Y), Convert.ToInt32(ptBottomRightVedioScr.X), Convert.ToInt32(ptBottomRightVedioScr.Y));
            }
            else
            {
                boxControl.UpdateVedioRect(Convert.ToInt32(ptTopLeftVedioScr.X), Convert.ToInt32(ptTopLeftVedioScr.Y), Convert.ToInt32(ptBottomRightVedioScr.X), Convert.ToInt32(ptBottomRightVedioScr.Y));
            }
            /*********************转为屏幕WPF坐标*******************/
            System.Windows.Point ptTopLeftWait          = ptTopLeftVedio;
            System.Windows.Point ptBottomRightWait      = ptBotRightVedio;
            ptTopLeftVedio                              = wnd.PointFromScreen(ptTopLeftVedioScr);
            ptBotRightVedio                             = wnd.PointFromScreen(ptBottomRightVedioScr);
            if (WndState == WindowState.Maximized)
            {

            }
            else
            {
                ptTopLeftWait.X                         += WindowLeft;
                ptTopLeftWait.Y                         += WindowTop;
                ptBottomRightWait.X                     += WindowLeft;
                ptBottomRightWait.Y                     += WindowTop;

                ptTopLeftVedio.X                        += WindowLeft;
                ptTopLeftVedio.Y                        += WindowTop;
                ptBotRightVedio.X                       += WindowLeft;
                ptBotRightVedio.Y                       += WindowTop;
            }
            /******************************************************/

            /*********************设置窗口屏幕WPF坐标*******************/
            _waitWndModel.WindowLeft                    = ptTopLeftWait.X;
            _waitWndModel.WindowTop                     = ptTopLeftWait.Y;
            _waitWndModel.WindowWidth                   = boxGridWidth;
            _waitWndModel.WindowHeight                  = boxGridHeight;

            _vedioListModel.WindowLeft                  = ptTopLeftWait.X;
            _vedioListModel.WindowTop                   = ptTopLeftWait.Y;
            _vedioListModel.WindowWidth                 = boxGridWidth;
            _vedioListModel.WindowHeight                = boxGridHeight;

            _keyMAPModel.WindowLeft                     = ptTopLeftVedio.X;
            _keyMAPModel.WindowTop                      = ptTopLeftVedio.Y;
            _keyMAPModel.WindowWidth                    = (ptBotRightVedio.X - ptTopLeftVedio.X);
            _keyMAPModel.WindowHeight                   = (ptBotRightVedio.Y - ptTopLeftVedio.Y);

        }

        private string ShowTitleVersion(string addstr)
        {

            String titlestr     = "";
            object[] attributes = Assembly.GetExecutingAssembly().GetCustomAttributes(typeof(AssemblyTitleAttribute), false);
            if (attributes.Length > 0)
            {
                AssemblyTitleAttribute titleAttribute = (AssemblyTitleAttribute)attributes[0];
                titlestr = titleAttribute.Title;
            }
            int Major       = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Major;
            int Minor       = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Minor;
            int Build       = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Build;
            int Revision    = System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.Revision;

#if _TEST_VERSION
            return titlestr + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString() + addstr;
#else
            return titlestr + Major + "." + Minor + "." + Build + addstr;
#endif
        }

        private void DragWnd(object obj)
        {
            Window wnd = obj as Window;
           System.Windows.Point pp = Mouse.GetPosition(wnd);//WPF方法
           if(pp.X > 0 && pp.X < windowWidth && pp.Y > 0 && pp.Y < 18)
               wnd.DragMove();
        }

        private void MaxWnd(object obj)
        {
            _TitleHeight    = 0;
            _IsCollapsed    = true;
            boxControl.ClearTouchedKey();
            WndState = WindowState.Maximized;
        }
        private void NormalWnd(object obj)
        {
            _TitleHeight    = 18;
            _IsCollapsed    = false;
            boxControl.ClearTouchedKey();
            WndState = WindowState.Normal;
        }
        private void MinWnd(object obj)
        {
            boxControl.ClearTouchedKey();
            WndState = WindowState.Minimized;
        }
        public void ExitWnd(object obj)
        {
            try
            {
                boxControl.ClearTouchedKey();
                boxControl.Exit();                    ////控件优先退出
                _vedioListModel.ExitWnd(null);
                _waitWndModel.ExitWnd(null);
                _keyMAPModel.ExitWnd(null);

                SaveConfig();
                
                System.Windows.Application.Current.MainWindow.Close();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(GetType() + ":" + ex.Message);
            }
        }


        private void OnNotifyOnConnectStateChanged(bool obj)
        {
            IsConnect = obj;

            Window mainWindow = System.Windows.Application.Current.MainWindow;

            if (IsConnect)
            {
                if (IsHorizontalMode)
                {
                    //_vedioListModel.Rotate(4);
                    boxControl.UpdateDeviceSize(Common.Common.boxSize.Width, Common.Common.boxSize.Height); //盒子内部分辨率
                }
                else
                {
                    //_vedioListModel.Rotate(3);
                    boxControl.UpdateDeviceSize(Common.Common.boxSize.Height, Common.Common.boxSize.Width); //盒子内部分辨率
                }
                boxControl.SetFlag((int)Common.BoxFlags.BoxFlags_IsHorizontalMode, Convert.ToInt32(IsHorizontalMode));


                LoadConfig();
                LoadKeyMapping();
                UpdateWndPos(mainWindow);

                ConStatuText = "已连接";
                Title = ShowTitleVersion(" (" + _vedioListModel._curIPTemp + ")");

                _waitWndModel.WndVisible = Visibility.Hidden;
                _vedioListModel.WndVisible = Visibility.Hidden;
                mainWindow.Activate();
            }
            else
            {
                NormalWnd(mainWindow);
                ConStatuText = "连接已断开";

                _waitWndModel.WndVisible = Visibility.Visible;
                mainWindow.Activate();
            }
        }

        public void Rotate(object obj)
        {
            using (Graphics graphics = Graphics.FromHwnd(IntPtr.Zero))
            {
                NormalWnd(obj);
                if (isHorizontalMode)
                {
                    if (graphics.DpiX > 0)
                        WindowWidth             = 1280 * 96 / graphics.DpiX + _LEFTMENUWIDTH;
                    if (graphics.DpiY > 0)
                        WindowHeight            = 720 * 96 / graphics.DpiY  + 18;
                    WindowLeft  = (System.Windows.SystemParameters.PrimaryScreenWidth - WindowWidth) /2;
                    WindowTop   = (System.Windows.SystemParameters.PrimaryScreenHeight - WindowHeight) /2;

                    boxControl.SetFlag((int)Common.BoxFlags.BoxFlags_IsHorizontalMode, Convert.ToInt32(IsHorizontalMode));
                    _vedioListModel.Rotate(4);
                    boxControl.UpdateDeviceSize(Common.Common.boxSize.Width, Common.Common.boxSize.Height); //盒子内部分辨率
                }
                else
                {
                    double  _MAX_ANDRIOD_HEIGHT     = (System.Windows.SystemParameters.PrimaryScreenHeight - 100 - _TitleHeight);
                    double  _WIDTH_HEIGHT_RATIO     = 9 / 16.0;

                    WindowWidth                 = _MAX_ANDRIOD_HEIGHT * 1.0 * _WIDTH_HEIGHT_RATIO + _LEFTMENUWIDTH;
                    WindowHeight                = _MAX_ANDRIOD_HEIGHT * 1.0;

                    WindowLeft  = (System.Windows.SystemParameters.PrimaryScreenWidth - WindowWidth) /2;
                    WindowTop   = (System.Windows.SystemParameters.PrimaryScreenHeight - WindowHeight) /2;


                    boxControl.SetFlag((int)Common.BoxFlags.BoxFlags_IsHorizontalMode, Convert.ToInt32(IsHorizontalMode));
                    _vedioListModel.Rotate(3);
                    boxControl.UpdateDeviceSize(Common.Common.boxSize.Height, Common.Common.boxSize.Width); //盒子内部分辨率
                }

                //UpdateWndPos(obj);
            }
        }
        #endregion
    }
}
