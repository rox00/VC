using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using UYouMain.Common;

namespace UYouMain.ViewModel
{
    public class QueueData
    {
        private object                  syncRoot    = new object();
        private Queue                   _queue      = new Queue();
        private ManualResetEventSlim    _event      = new ManualResetEventSlim();

        public ManualResetEventSlim GetQueueEvent()
        {
            return _event;
        }

        public void Push(object obj)
        {
            lock (syncRoot)
            {
                _queue.Enqueue(obj);

                _event.Set();
            }
        }
        public int Pop(ref object obj)
        {
            lock (syncRoot)
            {
                if (_queue.Count <= 0)
                    return _queue.Count;

                obj = _queue.Dequeue();

                if (_queue.Count == 0)
                    _event.Reset();
                return _queue.Count;
            }
        }
        public int GetQueueCount()
        {
            lock (syncRoot)
            {
                return _queue.Count;
            }
        }
    }

    public enum PackTypeEnum : byte
    {
        /**********************状态区（UDP）0~99*****************************/
        PackTypeEnum_clientidle          = 0,
        PackTypeEnum_clientbusy          = 1,
        PackTypeEnum_serveridle          = 2,
        PackTypeEnum_serverbusy          = 3,
        PackTypeEnum_ImgData             = 99,

        //PackTypeEnum_rebootask           = 50,          //去掉
        //PackTypeEnum_rebootcan           = 51,          //去掉
        //PackTypeEnum_rebootok            = 52,          //去掉

        //PackTypeEnum_askconnect          = 4,           //去掉
        //PackTypeEnum_allowconnect        = 5,           //去掉
        //PackTypeEnum_refuseconnect       = 6,           //去掉
        //PackTypeEnum_forcereboot         = 1000,        //去掉

        /**********************功能区（短连接）100~199*****************************/
        PackTypeEnum_installfile        = 100,         //安装文件     升级文件 uyouupgrade.zip 角本名称uyouupgrade.sh
        PackTypeEnum_installok          = 101,         //安装文件成功
        PackTypeEnum_installfail        = 102,         //安装文件失败

        PackTypeEnum_rebootask          = 103,         //请求重启设备
        PackTypeEnum_rebootok           = 104,         //重启成功
        PackTypeEnum_rebootfail         = 105,         //重启失败

        PackTypeEnum_updateclip         = 150,         //更新剪切板字符串  data=字符串
        PackTypeEnum_rotate             = 151,         //旋转屏幕 data=int值（0,90,180,270）


        /**********************视频控制（长连接）200~255*****************************/
        PackTypeEnum_VEDIO			    = 200,			//视频 
        PackTypeEnum_MOUSE			    = 201,			//鼠标
        PackTypeEnum_KEYBOARD		    = 202,			//键盘
    }

    [Serializable]
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct PackHead
    {
        //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        //public byte[]                   guid;
        public byte                     head1;                  //0xff
        public byte                     head2;                  //0xfe
        public byte                     type;                   //包类型
        public int                      datalen;                //包长度
    }

    [Serializable]
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct PackInstallFile
    {
        public PackHead                 packHead;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
        public string                   fileName;               //文件路径长度260字节  ex: screenmirror.apk
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[]                   md5;                    //md5码
        public int                      fileLen;
    }

    public struct PackPasteString
    {
        public PackHead                 packHead;
        public String                   str;               //文本长度
    }

    [Serializable]
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
    public struct PackRotateAngle
    {
        public PackHead                 packHead;
        public int                      angle;               //文本长度
    }
 

    public class VedioListItemTemplateSelector : DataTemplateSelector
    {
        public DataTemplate DeviceTemplate { get; set; }
        public DataTemplate AddTemplate { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            FrameworkElement element = container as FrameworkElement;

            if (element != null && item != null && item is VedioCtrlModel)
            {
                VedioCtrlModel auctionItem = item as VedioCtrlModel;

                switch (auctionItem._CtrlType)
                {
                    case VedioCtrlType.VedioCtrlType_Device:
                        return DeviceTemplate;
                    case VedioCtrlType.VedioCtrlType_Add:
                        return AddTemplate;
                }
            }

            return null;
        }
    }

    public class IPValidateRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            if (value == null)
                return new ValidationResult(false, "不能为空值！");
            if (string.IsNullOrEmpty(value.ToString()))
                return new ValidationResult(false, "不能为空字符串！");

            try
            {
                string newValue = (string)value;
                System.Net.IPAddress ipaddress = System.Net.IPAddress.Parse(newValue);
            }
            catch (Exception e)
            {
                string str = e.Message;
                Debug.WriteLine(GetType() + ":" + str);

                return new ValidationResult(false, "不可解析的字符串");
            }


            return new ValidationResult(true, null);
        }
    }

    enum VedioCtrlType
    {
        VedioCtrlType_Device    = 0,
        VedioCtrlType_Add       = 1,
    }

    class VedioCtrlModel : NotificationObject
    {
        Dispatcher                              thisDispather { get; set;}
        /****************************************************/
        private string                          version;
        public string Version
        {
            get { return version; }
            set
            {
                version = value;
                this.RaisePropertyChanged("Version");
            }
        }
        /****************************************************/

        /****************************************************/
        private string                          ipString;
        public string IpString
        {
            get { return ipString; }
            set
            {
                ipString = value;
                this.RaisePropertyChanged("IpString");
            }
        }
        /****************************************************/
        /****************************************************/
        private string                          remark;
        public string Remark
        {
            get { return remark; }
            set
            {
                remark = value;
                this.RaisePropertyChanged("IpString");
            }
        }
        /****************************************************/
        /****************************************************/
        private bool                          isBusy;
        public bool IsBusy
        {
            get { return isBusy; }
            set
            {
                isBusy = value;
                this.RaisePropertyChanged("IsBusy");
            }
        }
        /****************************************************/

        /****************************************************/
        private bool                          isFindDevice;
        public bool IsFindDevice
        {
            get { return isFindDevice; }
            set
            {
                isFindDevice = value;
                this.RaisePropertyChanged("IsFindDevice");
            }
        }
        /****************************************************/

        /****************************************************/
        private string                          statusText;
        public string StatusText
        {
            get { return statusText; }
            set
            {
                statusText = value;
                this.RaisePropertyChanged("StatusText");
            }
        }
        /****************************************************/
        /****************************************************/
        private BitmapImage                          scrShotImg;
        public BitmapImage ScrShotImg
        {
            get { return scrShotImg; }
            set
            {
                scrShotImg = value;
                this.RaisePropertyChanged("ScrShotImg");
            }
        }
        /****************************************************/
        /****************************************************/
        private bool                          isSelect;
        public bool IsSelect
        {
            get { return isSelect; }
            set
            {
                isSelect = value;
                this.RaisePropertyChanged("IsSelect");
            }
        }
        /****************************************************/
        /****************************************************/
        private int                          progressValue;
        public int ProgressValue
        {
            get { return progressValue; }
            set
            {
                progressValue = value;
                this.RaisePropertyChanged("ProgressValue");
            }
        }
        /****************************************************/
        /****************************************************/
        private bool                            isProgressing;
        public bool IsProgressing
        {
            get { return isProgressing; }
            set
            {
                isProgressing = value;
                this.RaisePropertyChanged("IsProgressing");
            }
        }
        /****************************************************/

        public DelegateCommand<object>          LoadCommand { get; set; }
        public DelegateCommand<object>          leftMouseClickCommand { get; set; }
        public DelegateCommand<object>          rightMouseClickCommand { get; set; }
        public DelegateCommand<object>          deleteCommand { get; set; }
        public DelegateCommand<object>          connectCommand { get; set; }
        public DelegateCommand<object>          addCommand { get; set; }


        private Task            _task           = new Task(() => { });
        private  QueueData      _queuedata      = new QueueData();
        public  DateTime        _time           = DateTime.Now;
        public  VedioCtrlType   _CtrlType       = VedioCtrlType.VedioCtrlType_Device;

        public VedioCtrlModel(VedioCtrlType type = VedioCtrlType.VedioCtrlType_Device)
        {
            _CtrlType                           = type;

            version                             = "";
            ipString                            = "127.0.0.1";
            remark                              = "测试设备";
            isBusy                              = false;
            isFindDevice                        = false;
            statusText                          = "无法获取设备信息!";
            scrShotImg                          = null;
            isSelect                            = false;
            progressValue                       = 0;
            isProgressing                       = false;

            //if(type == VedioCtrlType.VedioCtrlType_Add)
            //{
            //    ipString    = "127.0.0.1";
            //    remark      = "测试设备";
            //}

            LoadCommand                         = new DelegateCommand<object>(new Action<object>(this.ViewLoaded));
            leftMouseClickCommand               = new DelegateCommand<object>(new Action<object>(this.OnLeftMouseDown));
            rightMouseClickCommand              = new DelegateCommand<object>(new Action<object>(this.OnRightMouseDown));

            _task.Start();
        }

        protected void ViewLoaded(object obj)
        {
            DispatcherObject dispacherobj = obj as DispatcherObject;
            thisDispather = dispacherobj.Dispatcher;
        }

        protected void OnLeftMouseDown(object obj)
        {
            IsSelect = !IsSelect;

            if (IsSelect == false && IsFindDevice)
            {
                if (IsProgressing == true && ProgressValue < 100)
                {////还没有传输完成
                    ;
                }
                else
                {////传输完成及其它情况
                    IsProgressing    = false;
                    ProgressValue   = 0;
                    StatusText      = "";
                }
            }
        }
        protected void OnRightMouseDown(object obj)
        {
            IsSelect = false;
        }

        protected void processPackData(Task task)
        {//为了使用TASK
            if (_queuedata.GetQueueCount() <= 0)
                return;

            object obj  = null;
            _queuedata.Pop(ref obj);
            KeyValuePair<String, object> pair = (KeyValuePair<String, object>)obj;

            if (pair.Value.GetType() == typeof(PackInstallFile))
            {   ////安装文件
                PackInstallFile packFile                    = (PackInstallFile)pair.Value;
                string          apkPath                     = packFile.fileName;


                startProgressing("正在发送文件……");
                FileInfo        fileInfo                    = new FileInfo(apkPath);
                FileStream      fileStrem                   = new FileStream(apkPath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);

                try
                {
                    System.Security.Cryptography.MD5 md5    = new System.Security.Cryptography.MD5CryptoServiceProvider();
                    byte[] md5Array                         = md5.ComputeHash(fileStrem);

                    packFile.packHead.head1                 = 0xFF;
                    packFile.packHead.head2                 = 0xFE;
                    packFile.packHead.type                  = (byte)PackTypeEnum.PackTypeEnum_installfile;
                    packFile.packHead.datalen               = Marshal.SizeOf(typeof(PackInstallFile)) - Marshal.SizeOf(typeof(PackHead));
                    packFile.fileName                       = fileInfo.Name;
                    packFile.fileLen                        = (Int32)fileInfo.Length;
                    packFile.md5                            = md5Array;

                    NetworkStream       workStream          = null;
                    TcpClient           tcpClient           = new TcpClient();
                    tcpClient.ReceiveTimeout                = 60000 * 5;   //10秒;
                    tcpClient.SendTimeout                   = 60000 * 5;   //10秒;
                    tcpClient.ReceiveBufferSize             = 4096;
                    tcpClient.SendBufferSize                = 4096;
                    tcpClient.Connect(new IPEndPoint(IPAddress.Parse(pair.Key), 12581));
                    workStream = tcpClient.GetStream();

                    ////写文件包信息
                    byte[] data = Common.Common.StructToBytes(packFile);
                    workStream.Write(data, 0, data.Length);

                    ////发送文件
                    int     readSize    = 0;//初始化读取的流量为0   
                    int     len         = 0;//初始化已经读取的流量   
                    byte[]  databuf     = new byte[4096];
                    fileStrem.Seek(0, SeekOrigin.Begin);
                    while (len < fileStrem.Length)
                    {
                        readSize = fileStrem.Read(databuf, 0, databuf.Length);
                        workStream.Write(databuf, 0, readSize);
                        len += readSize;

                        updateProgressing("正在发送文件……", Convert.ToInt32(len * 1.0/fileStrem.Length * 90));
                    }

                    fileStrem.Close();

                    updateProgressing("正在安装文件……", 92);
                    readSize = workStream.Read(databuf, 0, databuf.Length);

                    if (fileInfo.Name == "uyouupgrade.zip")
                    {
                        PackHead packRev = (PackHead)Common.Common.BytesToStruct(databuf, typeof(PackHead));

                        if (packRev.type == (byte)PackTypeEnum.PackTypeEnum_installok)
                        {
                            int datalen = packRev.datalen;
                            string str = System.Text.Encoding.Default.GetString(databuf, Marshal.SizeOf(typeof(PackHead)), readSize - Marshal.SizeOf(typeof(PackHead)));
                            bool bb = str.Contains("end uyouupgrade.sh");

                            if (str.Contains("can't") || str.Contains("error"))
                            {
                                badProgressing("升级失败！");
                            }
                            else
                            {
                                updateProgressing("安装文件成功！", 100);
                            }
                        }
                        else
                        {
                            badProgressing("安装文件失败！包类型：" + packRev.type);
                        }
                    }
                    else
                    {
                        if (readSize == Marshal.SizeOf(typeof(PackHead)))
                        {
                            PackHead packRev = (PackHead)Common.Common.BytesToStruct(databuf, typeof(PackHead));
                            if (packRev.type == (byte)PackTypeEnum.PackTypeEnum_installok)
                            {
                                updateProgressing("安装文件成功！", 100);
                            }
                            else
                            {
                                badProgressing("安装文件失败！包类型：" + packRev.type);
                            }
                        }
                        else
                        {
                            badProgressing("安装文件失败：接收数据长度不一致！");
                        }
                    }
                }
                catch (Exception e)
                {
                    badProgressing(e.Message);
                }
            }
            else if (pair.Value.GetType() == typeof(PackHead))
            {
                PackHead packHead               = (PackHead)pair.Value;

                try
                {
                    if (packHead.type == (byte)PackTypeEnum.PackTypeEnum_rebootask)
                    {
                        startProgressing("正在重启……");
                        packHead.head1                          = 0xFF;
                        packHead.head2                          = 0xFE;
                        packHead.datalen                        = 0;

                        NetworkStream       workStream          = null;
                        TcpClient           tcpClient           = new TcpClient();
                        tcpClient.ReceiveTimeout                = 120000;   //10秒;
                        tcpClient.SendTimeout                   = 120000;   //10秒;
                        tcpClient.ReceiveBufferSize             = 4096;
                        tcpClient.SendBufferSize                = 4096;
                        tcpClient.Connect(new IPEndPoint(IPAddress.Parse(pair.Key), 12581));
                        workStream = tcpClient.GetStream();

                        byte[] data = Common.Common.StructToBytes(packHead);
                        workStream.Write(data, 0, data.Length);

                        updateProgressing("正在重启……", 40);
                        byte[]  databuf     = new byte[4096];
                        int readSize        = workStream.Read(databuf, 0, databuf.Length);

                        if (readSize == Marshal.SizeOf(typeof(PackHead)))
                        {
                            PackHead packRev = (PackHead)Common.Common.BytesToStruct(databuf, typeof(PackHead));
                            if (packRev.type == (byte)PackTypeEnum.PackTypeEnum_rebootok)
                            {
                                updateProgressing("重启成功！", 100);
                                IsBusy = true;
                            }
                            else
                            {
                                badProgressing("重启失败！包类型：" + packRev.type);
                            }
                        }
                        else
                        {
                            badProgressing("重启失败：接收数据长度不一致！");
                        }
                    }

                }
                catch (Exception e)
                {
                    badProgressing(e.Message);
                }
            }
            else if (pair.Value.GetType() == typeof(PackPasteString))
            {
                PackPasteString packpastestring                 = (PackPasteString)pair.Value;

                try
                {
                    byte[] strBytes = Encoding.UTF8.GetBytes(packpastestring.str);

                    packpastestring.packHead.head1                  = 0xFF;
                    packpastestring.packHead.head2                  = 0xFE;
                    packpastestring.packHead.type                   = (byte)PackTypeEnum.PackTypeEnum_updateclip;
                    packpastestring.packHead.datalen                = strBytes.Length;


                    NetworkStream       workStream                  = null;
                    TcpClient           tcpClient                   = new TcpClient();
                    tcpClient.ReceiveTimeout                        = 120000;   //10秒;
                    tcpClient.SendTimeout                           = 120000;   //10秒;
                    tcpClient.ReceiveBufferSize                     = 4096;
                    tcpClient.SendBufferSize                        = 4096;
                    tcpClient.Connect(new IPEndPoint(IPAddress.Parse(pair.Key), 12581));
                    workStream = tcpClient.GetStream();

                    byte[] data = Common.Common.StructToBytes(packpastestring.packHead);
                    workStream.Write(data, 0, data.Length);
                    workStream.Write(strBytes, 0, strBytes.Length);
                }
                catch (Exception e)
                {
                    Debug.WriteLine(GetType() + ":" + e.Message);
                }
            }
            else if (pair.Value.GetType() == typeof(PackRotateAngle))
            {
                PackRotateAngle packrotateangle                 = (PackRotateAngle)pair.Value;

                try
                {
                    packrotateangle.packHead.head1                  = 0xFF;
                    packrotateangle.packHead.head2                  = 0xFE;
                    packrotateangle.packHead.type                   = (byte)PackTypeEnum.PackTypeEnum_rotate;
                    packrotateangle.packHead.datalen                = Marshal.SizeOf(typeof(int));


                    NetworkStream       workStream                  = null;
                    TcpClient           tcpClient                   = new TcpClient();
                    tcpClient.ReceiveTimeout                        = 120000;   //10秒;
                    tcpClient.SendTimeout                           = 120000;   //10秒;
                    tcpClient.ReceiveBufferSize                     = 4096;
                    tcpClient.SendBufferSize                        = 4096;
                    tcpClient.Connect(new IPEndPoint(IPAddress.Parse(pair.Key), 12581));
                    workStream = tcpClient.GetStream();

                    byte[] data = Common.Common.StructToBytes(packrotateangle);
                    workStream.Write(data, 0, data.Length);
                }
                catch (Exception e)
                {
                    Debug.WriteLine(GetType() + ":" + e.Message);
                }
            }
        }

        protected void startProgressing(string statutext)
        {
            IsProgressing    = true;
            StatusText       = statutext;
        }

        protected void updateProgressing(string statutext, int value)
        {
            ProgressValue   = value;
            StatusText      = statutext;
        }

        protected void badProgressing(string statutext)
        {
            IsProgressing    = false;
            StatusText      = statutext;
        }

        public void InstallAPK(string apkPath)
        {
            PackInstallFile    packFile             = new PackInstallFile();
            packFile.fileName                       = apkPath;

            KeyValuePair<String, object> pair       = new KeyValuePair<string, object>(ipString, packFile);
            _queuedata.Push(pair);
            _task = _task.ContinueWith(processPackData, TaskContinuationOptions.LongRunning);
        }

        public void RebootDevice()
        {
            PackHead    packHead                    = new PackHead();
            packHead.type                           = (byte)PackTypeEnum.PackTypeEnum_rebootask;


            KeyValuePair<String, object> pair       = new KeyValuePair<string, object>(ipString, packHead);
            _queuedata.Push(pair);
            _task = _task.ContinueWith(processPackData, TaskContinuationOptions.LongRunning);
        }


        public void PasteStr(String str)
        {
            PackPasteString    packpastestring      = new PackPasteString();
            packpastestring.str                     = str;


            KeyValuePair<String, object> pair       = new KeyValuePair<string, object>(ipString, packpastestring);
            _queuedata.Push(pair);
            _task = _task.ContinueWith(processPackData, TaskContinuationOptions.LongRunning);
        }

        public void Rotate(Int32 angle)
        {
            PackRotateAngle    packrotateangle      = new PackRotateAngle();
            packrotateangle.angle                   = angle;


            KeyValuePair<String, object> pair       = new KeyValuePair<string, object>(ipString, packrotateangle);
            _queuedata.Push(pair);
            _task = _task.ContinueWith(processPackData, TaskContinuationOptions.LongRunning);
        }
    }
}
