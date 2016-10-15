using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using System.Windows.Interop;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using System.Xml;
using UYouMain.Common;

namespace UYouMain.ViewModel
{
    class VedioListModel : NotificationObject
    {
        Dispatcher                                  thisDispather { get; set; }
        /****************************************************/
        private ObservableCollection<VedioCtrlModel> vedioCtrArray;
        public ObservableCollection<VedioCtrlModel> VedioCtrArray
        {
            get
            {
                return vedioCtrArray;
            }
            set
            {
                vedioCtrArray = value;
                this.RaisePropertyChanged("VedioCtrArray");
            }
        }
        /****************************************************/

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

        public DelegateCommand<object>              LoadCommand { get; set; }
        public DelegateCommand<object>              DeviceInfo2LocalCommand { get; set; }
        public DelegateCommand<object>              DeviceInfo2ServerCommand { get; set; }
        public DelegateCommand<object>              InstallFileCommand{ get; set; }
        public DelegateCommand<object>              RebootCommand { get; set; }


        static int                                  _udpPort            = 12306;
        UdpClient                                   _udpClient          = new UdpClient(0);
        Task                                        _task               = new Task(() => { });
        public QueueData                            _QueueData          = new QueueData();
        List<String>                                _ipList             = new List<String>();
        System.Net.IPAddress[]                      broadAddressList;

        public Func<uint, uint, string, uint>       Start;
        private Action<String>                      pasteAction;
        private Action<Int32>                       rotateAction;
        public string                               _curIPTemp      = "";
        public bool                                 _bExit      = false;

        public VedioListModel()
        {
            #region 初始化网络相关信息
            broadAddressList                = getBroadcasetAddress();
            uint IOC_IN                     = 0x80000000;
            uint IOC_VENDOR                 = 0x18000000;
            uint SIO_UDP_CONNRESET          = IOC_IN | IOC_VENDOR | 12;
            _udpClient.Client.IOControl((int)SIO_UDP_CONNRESET, new byte[] { Convert.ToByte(false) }, null);
            #endregion

            wndVisible                      = Visibility.Hidden;
            windowLeft                      = 0;
            windowTop                       = 0;
            windowWidth                     = 0;
            windowHeight                    = 0;

            vedioCtrArray                   = new ObservableCollection<VedioCtrlModel>();
            LoadCommand                     = new DelegateCommand<object>(new Action<object>(this.ViewLoaded));
            DeviceInfo2LocalCommand         = new DelegateCommand<object>(new Action<object>(this.updateDevInfoToLocal));
            DeviceInfo2ServerCommand        = new DelegateCommand<object>(new Action<object>(this.updateDevInfoToSrv));
            InstallFileCommand              = new DelegateCommand<object>(new Action<object>(this.InstallApk));
            RebootCommand                   = new DelegateCommand<object>(new Action<object>(this.RebootDevice));

            ReLoadVedioArray();
        }
        /*********************自定义函数*****************************/
        private IPAddress[] getBroadcasetAddress()
        {
            ArrayList arr = new ArrayList();

            ManagementObjectSearcher    query = new ManagementObjectSearcher("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = 'TRUE'");
            ManagementObjectCollection  queryCollection = query.Get();
            foreach (ManagementObject mo in queryCollection)
            {
                string[] addresses          = (string[])mo["IPAddress"];
                string[] subnets            = (string[])mo["IPSubnet"];
                string[] defaultgateways    = (string[])mo["DefaultIPGateway"];

                byte[] tip = IPAddress.Parse(addresses[0]).GetAddressBytes();
                byte[] tsub = IPAddress.Parse(subnets[0]).GetAddressBytes();

                for (int i = 0; i < tip.Length; i++)
                    tip[i] = (byte)((~tsub[i])|tip[i]);

                arr.Add(new IPAddress(tip));
            }

            IPAddress[] ret = new IPAddress[arr.Count];
            int count = 0;
            foreach (Object obj in arr)
            {
                ret[count++] = (IPAddress)obj;
            }
            return ret;
        }
        private void ProcessData()
        {//为了使用TASK
            try
            {
                object obj  = null;
                _QueueData.Pop(ref obj);

                KeyValuePair<string, Byte[]> pair   = (KeyValuePair<string, byte[]>)obj;
                string  ipstring                    = pair.Key;
                Byte [] receiveBytes                = pair.Value;

                PackHead packRev = (PackHead)Common.Common.BytesToStruct(receiveBytes, typeof(PackHead));
                if (packRev.type == (byte)PackTypeEnum.PackTypeEnum_clientbusy    || 
                        packRev.type == (byte)PackTypeEnum.PackTypeEnum_clientidle    ||
                        packRev.type == (byte)PackTypeEnum.PackTypeEnum_ImgData
                )
                {
                    for (int i = 0; i < vedioCtrArray.Count; ++i)
                    {
                        VedioCtrlModel   vediomodeTemp          = vedioCtrArray[i] as VedioCtrlModel;
                        if (vediomodeTemp._CtrlType == VedioCtrlType.VedioCtrlType_Device)
                        {
                            if (vediomodeTemp == null) continue;
                            VedioCtrlModel      vediomode               = null; ////用来赋值处理消息的控件
                            int                 compare                 = ipstring.CompareTo(vediomodeTemp.IpString);
                            if (compare == 0)
                            {   ////如果刚好找到设备
                                vediomode                       = vediomodeTemp;
                                vediomode._time                 = DateTime.Now;
                                if (vediomode.IsFindDevice == false)
                                    vediomode.StatusText = "";  //状态切换

                                vediomode.IsFindDevice           = true;

                                if (packRev.type == (byte)PackTypeEnum.PackTypeEnum_ImgData)
                                {   //如果是图片

                                    thisDispather.Invoke(new Action(() =>
                                    {   ////图片应当在UI线程中创建
                                        try
                                        {
                                            BitmapImage bmp = new BitmapImage();
                                            bmp.BeginInit();
                                            bmp.StreamSource = new MemoryStream(receiveBytes, Marshal.SizeOf(packRev), packRev.datalen);
                                            bmp.EndInit();
                                            vediomode.ScrShotImg = bmp;
                                        }
                                        catch (Exception e)
                                        {
                                            string str = e.Message;
                                            Debug.WriteLine(GetType() + ":" + str);
                                        }
                                    }));
                                }
                                else
                                {
                                    vediomode.IsBusy            = packRev.type == (byte)PackTypeEnum.PackTypeEnum_clientbusy;

                                    string str = System.Text.Encoding.Default.GetString(receiveBytes, Marshal.SizeOf(typeof(PackHead)), receiveBytes.Length - Marshal.SizeOf(typeof(PackHead)));
                                    vediomode.Version = str;
                                }
                                break;
                            }
                        }
                    }
                }
            }
            catch { }
        }
        private void ReceiveCallback(IAsyncResult ar)
        {
            UdpClient u                                 = (UdpClient)(ar.AsyncState);
            IPEndPoint pt                               = new IPEndPoint(IPAddress.Any, _udpPort);
            try
            {
                Byte[] receiveBytes                     = u.EndReceive(ar, ref pt);
                string ipstring                         = pt.Address.ToString();

                if (receiveBytes[0] == 0xFF && receiveBytes[1] == 0xFE)
                {
                    _QueueData.Push(new KeyValuePair<string, Byte[]>(ipstring, receiveBytes));
                    Debug.WriteLine(String.Format("当前队列数 {0} is doing something", _QueueData.GetQueueCount()));
                }
                Debug.WriteLine("ip:{0}\t\t\t\trecevielen:{1}\n", ipstring, receiveBytes.Length);
            }
            catch
            { }

            u.BeginReceive(new AsyncCallback(ReceiveCallback), u);
        }


        protected void ViewLoaded(object obj)
        {
            DispatcherObject dispacherobj = obj as DispatcherObject;
            thisDispather = dispacherobj.Dispatcher;


            _udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), _udpClient);

            #region 处理数据
            Thread tProcess = new Thread(() =>
            {   ////循环查询过期IP并删除之
                ManualResetEventSlim    _event      = _QueueData.GetQueueEvent();
                while (!_bExit)
                {
                    _event.Wait();
                    ProcessData();
                }
                Debug.WriteLine(GetType() + ":" + "exit");
            }
            );
            tProcess.IsBackground = true;
            tProcess.Start();
            #endregion 处理数据

            #region 失效处理
            Thread tRemove = new Thread(() =>
            {   ////循环查询过期IP并删除之
                while (!_bExit)
                {
                    try
                    {
                        for (int i = 0; i < vedioCtrArray.Count; ++i)
                        {
                            VedioCtrlModel vediomodeTemp = vedioCtrArray[i] as VedioCtrlModel;
                            if (vediomodeTemp._CtrlType == VedioCtrlType.VedioCtrlType_Device)
                            {
                                if ((DateTime.Now - vediomodeTemp._time).TotalSeconds >= 25)
                                {
                                    vediomodeTemp.IsFindDevice  = false;
                                    vediomodeTemp.StatusText    = "无法获取设备信息!";
                                }
                            }
                        }

                        int timecnt = 0;
                        while (timecnt < 200 && !_bExit)
                        {

                            Thread.Sleep(50);
                            timecnt++;
                        }
                    }
                    catch
                    {

                    }
                }
                Debug.WriteLine(GetType() + ":" + "exit");
            }
            );
            tRemove.IsBackground = true;
            tRemove.Start();
            #endregion 删除
            #region 广播
            Thread tBroad = new Thread(() =>
            {   ////循环定期广播发送服务在线

                PackHead  pack          = new PackHead();
                pack.head1              = 0xFF;
                pack.head2              = 0xFE;
                pack.type               = (byte)PackTypeEnum.PackTypeEnum_serveridle;
                pack.datalen            = 0;
                byte[]      buffer      = Common.Common.StructToBytes(pack);

                while (!_bExit)
                {
                    try
                    {
                        for (int i = 0; i < vedioCtrArray.Count; i++)
                        {
                            VedioCtrlModel vediomode = vedioCtrArray[i] as VedioCtrlModel;

                            if (vediomode != null && vediomode._CtrlType == VedioCtrlType.VedioCtrlType_Device)
                            {
                                _udpClient.Send(buffer, buffer.Length, new IPEndPoint(IPAddress.Parse(vediomode.IpString), _udpPort));
                            }
                        }

                        int timecnt = 0;
                        while (timecnt < 200 && !_bExit)
                        {

                            Thread.Sleep(50);
                            timecnt++;
                        }
                    }
                    catch
                    {

                    }
                }

                Debug.WriteLine(GetType() + ":" + "exit");
            }
            );
            tBroad.IsBackground = true;
            tBroad.Start();
            #endregion 广播
        }

        public void ExitWnd(object obj)
        {
            _bExit = true;
            vedioCtrArray.Clear();
        }

        public void ReLoadVedioArray()
        {
            VedioCtrArray.Clear();

            String xmlPath  = System.Windows.Forms.Application.StartupPath + "\\iplist.xml";
            if (File.Exists(xmlPath))
            {
                XmlDocument xmlDoc = new XmlDocument();
                xmlDoc.Load(xmlPath);

                XmlNodeList list = xmlDoc.SelectSingleNode("iplist").ChildNodes;    //获得根节点iplist下的所有子节点
                foreach (XmlNode node in list)
                {
                    XmlElement xe   = (XmlElement)node;                               //把节点node转换成XmlElement型
                    string ip       = xe.GetAttribute("ip");
                    string remark   = xe.GetAttribute("remark");

                    VedioCtrlModel vedioMode        = new VedioCtrlModel();
                    vedioMode.IpString              = ip;
                    vedioMode.IsBusy                = true;
                    vedioMode.Remark                = remark;
                    vedioMode.deleteCommand         = new DelegateCommand<object>(new Action<object>(this.DeleteChidren));
                    vedioMode.connectCommand        = new DelegateCommand<object>(new Action<object>(this.Connect));
                    VedioCtrArray.Add(vedioMode);
                }
            }

            VedioCtrlModel vedioctraddModel         = new VedioCtrlModel(VedioCtrlType.VedioCtrlType_Add);
            vedioctraddModel.addCommand             = new DelegateCommand<object>(new Action<object>(this.AddChidren));
            VedioCtrArray.Add(vedioctraddModel);
        }

        public void DeleteChidren(object obj)
        {
            VedioCtrArray.Remove(obj as VedioCtrlModel);
        }

        public void AddChidren(object obj)
        {
            VedioCtrlModel vedioctraddModel = obj as VedioCtrlModel;

            try
            {
                IPAddress addr = new IPAddress(0);
                if(IPAddress.TryParse(vedioctraddModel.IpString, out addr))
                {
                    VedioCtrlModel vedioMode        = new VedioCtrlModel();
                    vedioMode.IpString              = vedioctraddModel.IpString;
                    vedioMode.IsBusy                = true;
                    vedioMode.Remark                = vedioctraddModel.Remark;
                    vedioMode.deleteCommand         = new DelegateCommand<object>(new Action<object>(this.DeleteChidren));
                    vedioMode.connectCommand        = new DelegateCommand<object>(new Action<object>(this.Connect));

                    VedioCtrArray.Insert(VedioCtrArray.Count-1, vedioMode);
                }
            }
            catch
            {

            }
        }

        public void Connect(object obj)
        {
            VedioCtrlModel vediomode = obj as VedioCtrlModel;
            if (Start != null)
            {
                IntPtr intptr = new WindowInteropHelper(App.Current.MainWindow).Handle;
                Start(0, Convert.ToUInt32(intptr.ToInt32()), _curIPTemp = vediomode.IpString);
                pasteAction = vediomode.PasteStr;
                rotateAction = vediomode.Rotate;
            }
        }

        public void updateDevInfoToLocal(object obj)
        {
            // Show message box
            MessageBoxResult result = MessageBox.Show("本地设备列表已更新，重启软件后将自动加载！", "提示", MessageBoxButton.OK);
        }

        public void updateDevInfoToSrv(object obj)
        {
            String xmlPath  = System.Windows.Forms.Application.StartupPath + "\\iplist.xml";
            XmlTextWriter writer = new XmlTextWriter(xmlPath, System.Text.Encoding.UTF8);
            //使用自动缩进便于阅读
            writer.Formatting = Formatting.Indented;

            writer.WriteStartDocument();
            writer.WriteStartElement("iplist");

            for (int i = 0; i < VedioCtrArray.Count; i++)
            {
                VedioCtrlModel vediomode = VedioCtrArray[i] as VedioCtrlModel;

                if (vediomode != null && vediomode._CtrlType == VedioCtrlType.VedioCtrlType_Device)
                {
                    writer.WriteStartElement("ipinfo");
                    writer.WriteAttributeString("ip", vediomode.IpString);
                    writer.WriteAttributeString("remark", vediomode.Remark);
                    writer.WriteEndElement();
                }
            }

            writer.WriteEndElement();
            writer.WriteEndDocument();
            writer.Close();
        }

        public void InstallApk(object obj)
        {
            System.Windows.Forms.OpenFileDialog openFileDialog = new System.Windows.Forms.OpenFileDialog();
            //openFileDialog.InitialDirectory="c:\\";
            openFileDialog.Filter="*.APk(*.apk)|*.apk|*.ZIP(*.zip)|*.zip";

            openFileDialog.RestoreDirectory=true;

            if (openFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string fName=openFileDialog.FileName;

                for (int i = 0; i < VedioCtrArray.Count; i++)
                {
                    VedioCtrlModel vediomode = VedioCtrArray[i] as VedioCtrlModel;

                    if (vediomode != null)
                    {
                        if (vediomode.IsSelect && vediomode.IsFindDevice)
                        {
                            vediomode.InstallAPK(fName);
                        }
                    }
                }
            }
        }

        public void RebootDevice(object obj)
        {
            for (int i = 0; i < VedioCtrArray.Count; i++)
            {
                VedioCtrlModel vediomode = VedioCtrArray[i] as VedioCtrlModel;

                if (vediomode != null)
                {
                    if (vediomode.IsSelect && vediomode.IsFindDevice)
                    {
                        vediomode.RebootDevice();
                    }
                }
            }
        }

        public void PasteStr(String str)
        {
            try
            {
                pasteAction(str);
            }
            catch (Exception e)
            {
                Debug.WriteLine(GetType() + ":" + e.Message);
            }
        }

        public void Rotate(Int32 angle)
        {
            try
            {
                rotateAction(angle);
            }
            catch (Exception e)
            {
                Debug.WriteLine(GetType() + ":" + e.Message);
            }
        }
    }
}
