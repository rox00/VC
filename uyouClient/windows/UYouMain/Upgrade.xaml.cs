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
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;

using System.Threading;
using System.Runtime.Remoting.Messaging;
using System.Net;

namespace UYouMain
{    
    [DataContract]
    public class UpgrageConfig
    {        
        [DataMember]
        public string type;
        [DataMember]
        public string url;
        [DataMember]
        public string status;
        [DataMember]
        public string path;
        [DataMember]
        public string date;
        [DataMember]
        public string md5;
    }

    [DataContract]
    public class UpgradeRecord
    {
        [DataMember]
        public UpgrageConfig box;
        [DataMember]
        public UpgrageConfig pc;
    }
    /*
     * 1、pc—->ime—>7v
       pc到ime走的socket getUpdateInfo,发起升级请求
       ime到7v发送广播，com.ugame.upgrade.broadcast.ACTION,请求检测
     * 2、返回升级信息
        7v—>ime—>pc
     *  7v调用ime的aidl接口，onResult,传递升级信息给ime，ime走socket  发送android_to_pc_upgradeinfo命令给pc，PC解析升级信息，开始下载升级包
     * 3.1如果存在BOX升级，则PC在下载完PC端和Box端的软件后，通过 DownloadEnd事件通知主窗体，将文件Push到Box中，BOX升级成功后，发送EMV_ANDROID_TO_PC_UPGRADEFINISH命令给PC；
     *  PC端OCX控件接收到升级成功的事件后，判断是否需要升级PC端，如果有，则升级PC端；
     * 3.2如果不存在BOX升级，但存在PC升级，则下载完成后直接升级PC端
     */
    /// <summary>
    /// Upgrade.xaml 的交互逻辑
    /// </summary>
    public partial class Upgrade : Window
    {
        private WebClient                       MyWebClient_PC;
        private WebClient                       MyWebClient_Box;

        public delegate void                    DownloadEventHandler(Upgrade sender, DownloadEventArgs e);
        public event                            DownloadEventHandler DownloadEnd;//下载完成通知
        public delegate void                    DelegateStartUpgrade(String data);

        private delegate void                   UpdateTitle(string title, bool bSuccess);

        private UpdateTitle                     updateTitleHandler;
        private Emv_Oper_Upgrade_Data           upgradeData         = null;
        private UpgradeRecord                   _upgradeRecord      = new UpgradeRecord();
        private NLog.Logger                     logger              = NLog.LogManager.GetCurrentClassLogger();
        public UpgradeScope                     _upgradeScope       = UpgradeScope.NONE;


        public enum UpgradeScope
        {
            NONE,
            BOX,
            PC,
            BOTH
        }

        public enum UpgradeStatus {
            NONE,
            Updating,
            Done
        }

        public class DownloadEventArgs : EventArgs
        {
            private UpgradeScope _upgradeScope;
            public UpgradeScope UpgradeScope
            {
                get
                {
                    return _upgradeScope;
                }
            }

            private UpgrageConfig _upgrageConfig;
            public UpgrageConfig UpgrageConfig
            {
                get
                {
                    return _upgrageConfig;
                }
            }

            public DownloadEventArgs(UpgradeScope upgradeScope, UpgrageConfig upgrageConfig)
            {
                this._upgradeScope = upgradeScope;
                this._upgrageConfig = upgrageConfig;
            }
        }

        private void OnDownloadOver(DownloadEventArgs args)
        {
            if (_upgradeScope == UpgradeScope.BOX)
            {
                if (args.UpgradeScope == UpgradeScope.BOX)
                {
                    _upgradeScope = UpgradeScope.NONE;
                }
            }
            else if (_upgradeScope == UpgradeScope.PC)
            {
                if (args.UpgradeScope == UpgradeScope.PC)
                {
                    _upgradeScope = UpgradeScope.NONE;
                }
            }
            else if (_upgradeScope == UpgradeScope.BOTH)
            {
                if(args.UpgradeScope == UpgradeScope.BOX)
                {
                    _upgradeScope = UpgradeScope.PC;
                }
                else
                {
                    _upgradeScope = UpgradeScope.BOX;
                }
            }
                        
            if (_upgradeScope == UpgradeScope.NONE)
            {
                //文件下载完毕，开始升级
                if (_upgradeRecord.box != null)
                {
                    title.Dispatcher.BeginInvoke(updateTitleHandler, "正在进行系统升级，请保持连接", true);
                    logger.Info("正在进行系统升级，请保持连接");
                    progressBar.Visibility  = System.Windows.Visibility.Hidden;
                    btnOK.Visibility        = System.Windows.Visibility.Hidden;
                    btnCancel.Visibility    = System.Windows.Visibility.Hidden;
                    //15分钟后结束升级状态
                    System.Windows.Threading.DispatcherTimer timer = new System.Windows.Threading.DispatcherTimer();
                    timer.Interval          = TimeSpan.FromMinutes(15);
                    timer.Tick              += timer_Tick;
                    timer.Start();
                    //如果存在PC端的升级包，则会在push成功后，接收到Android通知后，在C++层执行PC端升级
                    DownloadEnd(this, args);   
                }
                else
                {
                    if (_upgradeRecord.pc != null)
                    {
                        DownloadEventArgs argPC = new DownloadEventArgs(UpgradeScope.PC, _upgradeRecord.pc);
                        DownloadEnd(this, argPC);
                    }
                }
                _startUpgrade = UpgradeStatus.Done;              
            }
        }

        void timer_Tick(object sender, EventArgs e)
        {
            System.Windows.Threading.DispatcherTimer timer = sender as System.Windows.Threading.DispatcherTimer;
            this._startUpgrade = UpgradeStatus.Done;
            title.Dispatcher.BeginInvoke(updateTitleHandler, "升级失败，请重新升级", false);            
            timer.Stop();
        }
      
        private void updateTitle(string title, bool bSuccess)
        {
            this.title.Text = title;
            if (bSuccess == false)
            {
                _startUpgrade = UpgradeStatus.Done;
            }
        }
        
        private void updateProgress(int current, int count)
        {
            progressBar.Maximum = count;
            progressBar.Value = current;
        }
                               
        public UpgradeStatus _startUpgrade
        {
            get { return (UpgradeStatus)this.GetValue(UpdateStatus); }
            set { this.SetValue(UpdateStatus, value); }
        }

        public static readonly DependencyProperty UpdateStatus =
            DependencyProperty.Register("UpdateStatus", typeof(UpgradeStatus), typeof(Upgrade),
            new FrameworkPropertyMetadata(UpgradeStatus.NONE, new PropertyChangedCallback(UpdateStatusPropertyChangedCallback)));

        private static void UpdateStatusPropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (sender != null && sender is Upgrade)
            {
                Upgrade clock = sender as Upgrade;
                clock.OnUpdatePropertyChanged((UpgradeStatus)arg.OldValue, (UpgradeStatus)arg.NewValue);

            }
        }

        protected virtual void OnUpdatePropertyChanged(UpgradeStatus oldValue, UpgradeStatus newValue)
        {
            RoutedPropertyChangedEventArgs<UpgradeStatus> arg =
                new RoutedPropertyChangedEventArgs<UpgradeStatus>(oldValue, newValue, UpdatePropertyChangedEvent);
            this.RaiseEvent(arg);

        }

        public static readonly RoutedEvent UpdatePropertyChangedEvent =
            EventManager.RegisterRoutedEvent("UpdateStatusPropertyChanged",
             RoutingStrategy.Bubble, typeof(RoutedPropertyChangedEventHandler<UpgradeStatus>), typeof(DirectionButton));

        public event RoutedPropertyChangedEventHandler<Upgrade> UpdateStatusPropertyChanged
        {
            add
            {
                this.AddHandler(UpdatePropertyChangedEvent, value);
            }
            remove
            {
                this.RemoveHandler(UpdatePropertyChangedEvent, value);
            }
        }
                
        public void Initialize(Emv_Oper_Upgrade_Data _data)
        {
            upgradeData             = _data;
            title.Text              = "检测到有新版本，是否升级？";

            btnOK.Visibility        = Visibility.Visible;
            btnReStart.Visibility   = Visibility.Hidden;
            btnCancel.Visibility    = Visibility.Visible;
            progressBar.Visibility  = Visibility.Visible;
            progressBar.Value       = 0;

            _startUpgrade           = UpgradeStatus.NONE;
            _upgradeScope           = UpgradeScope.NONE;
            _upgradeRecord          = new UpgradeRecord();

            this.Cursor             = Cursors.Arrow;
        }
        
        public Upgrade()
        {
            InitializeComponent();
            btnOK.Click             += btnOK_Click;
            btnCancel.Click         += btnCancel_Click;
            btnReStart.Click        += btnReStart_Click;
            btnOK.Visibility        = Visibility.Visible;
            btnReStart.Visibility   = Visibility.Hidden;
            btnCancel.Visibility    = Visibility.Visible;
            updateTitleHandler      = new UpdateTitle(updateTitle);            
        }

        void MyWebClient_Box_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            progressBar.Minimum     = 0;
            progressBar.Maximum     = e.TotalBytesToReceive;
            progressBar.Value       = e.BytesReceived;
        }

        void MyWebClient_Box_DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                //MessageBox.Show(e.Error.Message, "信息提示", MessageBoxButton.OK, MessageBoxImage.Information);
                //title.Text = "安卓端程序下载失败,请确认网络连接是否正常！";
                title.Text = "升级文件下载失败，请您先确认您的电脑以及小土豪主机的网络连接状态是否正常！";
                File.Delete(_upgradeRecord.box.path);
                
                if (MyWebClient_PC != null)
                {
                    if (MyWebClient_PC.IsBusy == true)
                    {
                        MyWebClient_PC.CancelAsync();
                    }
                }
                _startUpgrade           = UpgradeStatus.Done;
                btnReStart.Visibility   = Visibility.Visible;
                Cursor                  = Cursors.Arrow;
            }
            else
            {
                try
                {
                    FileStream fs = new FileStream(_upgradeRecord.box.path, FileMode.Open);
                    System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
                    byte[] retVal = md5.ComputeHash(fs);
                    fs.Close();
                    string str = BitConverter.ToString(retVal).Replace("-", String.Empty);
                    if (_upgradeRecord.box.md5 != null && _upgradeRecord.box.md5.ToLower().Trim() != str.ToLower().Trim())
                    {
                        title.Text = "安卓端程序校验失败，请重新下载";
                        if (MyWebClient_PC != null)
                        {
                            if (MyWebClient_PC.IsBusy == true)
                            {
                                MyWebClient_PC.CancelAsync();
                            }
                        }
                        _startUpgrade = UpgradeStatus.Done;
                        logger.Error(title.Text);
                        return;
                    }
                }
                catch (Exception ex)
                {
                    logger.Error(ex.Message);
                    _startUpgrade = UpgradeStatus.Done;
                    return;
                }

                DownloadEventArgs args = new DownloadEventArgs(UpgradeScope.BOX, _upgradeRecord.box);
                OnDownloadOver(args);
            }
        }

        void MyWebClient_PC_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            progressBar.Minimum = 0;
            progressBar.Maximum = e.TotalBytesToReceive;
            progressBar.Value = e.BytesReceived;
        }

        void MyWebClient_PC_DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                //MessageBox.Show(e.Error.Message, "信息提示", MessageBoxButton.OK, MessageBoxImage.Information);
                title.Text = "升级文件下载失败，请您先确认您的电脑以及小土豪主机的网络连接状态是否正常！";
                File.Delete(_upgradeRecord.pc.path);

                if (MyWebClient_Box != null)
                {
                    if (MyWebClient_Box.IsBusy)
                    {
                        MyWebClient_Box.CancelAsync();
                    }
                }
                _startUpgrade           = UpgradeStatus.Done;
                btnReStart.Visibility   = Visibility.Visible;
                Cursor                  = Cursors.Arrow;
            }
            else
            {
                try
                {
                    FileStream fs = new FileStream(_upgradeRecord.pc.path, FileMode.Open);
                    System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
                    byte[] retVal = md5.ComputeHash(fs);
                    fs.Close();
                    string str = BitConverter.ToString(retVal).Replace("-", String.Empty);

                    if (_upgradeRecord.pc.md5!= null && _upgradeRecord.pc.md5.ToLower().Trim() != str.ToLower().Trim())
                    {
                        title.Text = "PC端程序校验失败，请重新下载";
                        if (MyWebClient_Box != null)
                        {
                            if (MyWebClient_Box.IsBusy)
                            {
                                MyWebClient_Box.CancelAsync();
                            }
                        }
                        _startUpgrade = UpgradeStatus.Done;
                        logger.Error(title.Text);
                        return;
                    }
                }
                catch (Exception ex)
                {
                    logger.Error(ex.Message);
                    _startUpgrade = UpgradeStatus.Done;
                    return;
                }
                DownloadEventArgs args = new DownloadEventArgs(UpgradeScope.PC, _upgradeRecord.pc);
                OnDownloadOver(args);

                //PC端程序下载完成后，需要下载Box端
                if (upgradeData.box.downloadurl != null)
                {
                    DownloadBoxPackage();
                }
            }
        }        

        public static bool IsNullOrEmpty(string url){
            if (url == null ||
                url == "")
            {
                return true;
            }
            return false;
        }

        public static string GetUrlFileName(string url)
        {
            if (IsNullOrEmpty(url))
            {
                return "";
            }
            string[] strs1 = url.Split(new char[] { '/' });
            return strs1[strs1.Length - 1].Split(new char[] { '?' })[0];

        }  

        void btnCancel_Click(object sender, RoutedEventArgs e)
        {            
            Hide();            
        }
        
        private void DownloadBoxPackage()
        {
            title.Text                                      = "开始下载安卓端升级包";
            string configPath                               = System.Windows.Forms.Application.StartupPath + Common.Common.updataConfigFile;
            string downLoadPath                             = System.Windows.Forms.Application.StartupPath + Common.Common.updataFilePath;

            _upgradeRecord.box                              = new UpgrageConfig();
            _upgradeRecord.box.date                         = (new DateTime()).ToString();
            _upgradeRecord.box.path                         = downLoadPath + "\\" + GetUrlFileName(upgradeData.box.downloadurl);
            _upgradeRecord.box.url                          = upgradeData.box.downloadurl;
            _upgradeRecord.box.status                       = "1";
            _upgradeRecord.box.md5                          = upgradeData.box.md5;
            try
            {
                MyWebClient_Box                             = new WebClient();                
                MyWebClient_Box.DownloadFileCompleted       += MyWebClient_Box_DownloadFileCompleted;
                MyWebClient_Box.DownloadProgressChanged     += MyWebClient_Box_DownloadProgressChanged;
                MyWebClient_Box.Proxy                       = null;
                MyWebClient_Box.DownloadFileAsync(new Uri(_upgradeRecord.box.url), _upgradeRecord.box.path);
            }
            catch (Exception ex)
            {
                title.Text = "下载软件失败，请重新下载";
                logger.Error(ex.Message);
            }
        }

        private void DownloadPCPackage()
        {
            logger.Info("开始下载");

            title.Text                                      = "开始下载PC端升级包";
            string configPath                               = System.Windows.Forms.Application.StartupPath + Common.Common.updataConfigFile;
            string downLoadPath                             = System.Windows.Forms.Application.StartupPath + Common.Common.updataFilePath;

            _upgradeRecord.pc                               = new UpgrageConfig();
            _upgradeRecord.pc.date                          = (new DateTime()).ToString();
            _upgradeRecord.pc.path                          = downLoadPath + "\\" + GetUrlFileName(upgradeData.pc.downloadurl);
            _upgradeRecord.pc.url                           = upgradeData.pc.downloadurl;
            _upgradeRecord.pc.status                        = "1";
            _upgradeRecord.pc.md5                           = upgradeData.pc.md5;

            try
            {
                MyWebClient_PC                              = new WebClient();
                MyWebClient_PC.DownloadFileCompleted        += MyWebClient_PC_DownloadFileCompleted;
                MyWebClient_PC.DownloadProgressChanged      += MyWebClient_PC_DownloadProgressChanged;

                MyWebClient_PC.Proxy = null;
                MyWebClient_PC.DownloadFileAsync(new Uri(_upgradeRecord.pc.url), _upgradeRecord.pc.path, this);
            }
            catch (Exception ex)
            {
                title.Text = "下载软件失败，请重新下载";
                logger.Error(_upgradeRecord.pc.url + ex.Message);
            }
        }

        public void downloadPackage()
        {            
            if (_upgradeRecord == null)
            {
                _upgradeRecord = new UpgradeRecord();
            }            

            if (upgradeData.box.downloadurl != null && upgradeData.pc.downloadurl != null)
            {
                _upgradeScope = UpgradeScope.BOTH;
            }
            else if (upgradeData.box.downloadurl != null)
            {
                _upgradeScope = UpgradeScope.BOX;
            }
            else if (upgradeData.pc.downloadurl != null)
            {
                _upgradeScope = UpgradeScope.PC;
            }
            else
            {
                _upgradeScope = UpgradeScope.NONE;
            }

            if (upgradeData.pc.downloadurl != null)
            {
                DownloadPCPackage();                         
            }
            
            if (upgradeData.pc.downloadurl == null && upgradeData.box.downloadurl != null)
            {
                DownloadBoxPackage();                
            }
        }

        void startUpgrade()
        {
            if (_startUpgrade == UpgradeStatus.Updating)
            {
                return;
            }
            Directory.CreateDirectory(System.Windows.Forms.Application.StartupPath + "\\" + Common.Common.updataFilePath);
            if (_startUpgrade == UpgradeStatus.Done || _startUpgrade == UpgradeStatus.NONE)
            {
                Cursor                  = Cursors.Wait;
                downloadPackage();
                _startUpgrade           = UpgradeStatus.Updating;
                btnOK.Visibility        = Visibility.Hidden;
                btnCancel.Visibility    = Visibility.Hidden;
            }
            else
            {

            }
        }

        void btnOK_Click(object sender, RoutedEventArgs e)
        {
            startUpgrade();                                                
        }
               
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {            
            if(Visibility == Visibility.Visible)
            {
                e.Cancel = true;
                if (_startUpgrade == UpgradeStatus.Updating)
                {
                    MessageBox.Show("正在升级系统，请勿关闭");
                }
                else
                {
                    this.Hide();
                }
            }
        }
        private void btnReStart_Click(object sender, RoutedEventArgs e)
        {
            btnReStart.Visibility        = System.Windows.Visibility.Hidden;
            startUpgrade();
        }
    }
}
