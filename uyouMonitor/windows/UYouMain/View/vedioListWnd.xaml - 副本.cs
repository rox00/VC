using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
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
    /// vedioListWnd.xaml 的交互逻辑
    /// </summary>
    public partial class vedioListWnd : Window
    {
        public delegate void            DelegateStartConnect(string ipStr);
        static int                      _udpPort        = 12306;
        UdpClient                       _udpClient      = new UdpClient(_udpPort);
        public DelegateStartConnect     _StartConnect;
        public vedioListWnd()
        {
            InitializeComponent();
            this.Loaded             += MainWindow_Loaded;
        }

        public BitmapImage ByteArrayToBitmapImage(byte[] byteArray)
        {
            BitmapImage bmp = null;

            try
            {
                bmp = new BitmapImage();
                bmp.BeginInit();
                bmp.StreamSource = new MemoryStream(byteArray);
                bmp.EndInit();
            }
            catch
            {
                bmp = null;
            }

            return bmp;
        } 
         
        public void ReceiveCallback(IAsyncResult ar)
        {
            UdpClient u             = (UdpClient)(ar.AsyncState);
            IPEndPoint pt           = new IPEndPoint(IPAddress.Any, _udpPort);

            ////Byte[] receiveBytes     = u.EndReceive(ar, ref pt);
            //string ipstring         = pt.Address.ToString();
            //string receiveString1    = Encoding.ASCII.GetString(receiveBytes);
            //int count = 0;
                    //this.wrapPanel.Dispatcher.Invoke(new Action(() =>
                    //{
                    //    count = wrapPanel.Children.Count;
                    //}
                    //));

            //if (receiveBytes.Length > 256)
            //{   ////字节流转图片
            //    BitmapImage bmp = null;

            //    try
            //    {
            //        bmp = new BitmapImage();
            //        bmp.BeginInit();
            //        bmp.StreamSource = new MemoryStream(receiveBytes);
            //        bmp.EndInit();

            //        for (int i = 0; i < wrapPanel.Children.Count; ++i)
            //        {
            //            vedioCtrl vedioctrlTemp = wrapPanel.Children[i] as vedioCtrl;
            //            int compare = ipstring.CompareTo((vedioctrlTemp.btnText.Content as String));
            //            if (compare == 0)
            //            {   ////如果刚好找到设备
            //                vedioctrlTemp.img.Source = bmp;
            //                break;
            //            }
            //        }
            //    }
            //    catch
            //    {
            //        bmp = null;
            //    }
            //}
            //else 
            //{   ////收到上线信息
            //    string receiveString    = Encoding.ASCII.GetString(receiveBytes);
            //    if (receiveString == "uyouclient=online")
            //    {
            //        bool b = this.Dispatcher.CheckAccess();
            //        this.Dispatcher.Invoke(new Action(() =>
            //        {
            //            if (wrapPanel.Children.Count == 0)
            //            {
            //                vedioCtrl vedioctrl         = new vedioCtrl();
            //                vedioctrl.Width             = 100;
            //                vedioctrl.Height            = 100;
            //                vedioctrl._time             = DateTime.Now;
            //                vedioctrl.MouseDoubleClick  +=vedioctrl_MouseDoubleClick;
            //                vedioctrl.btnText.Content   = ipstring;
            //                wrapPanel.Children.Add(vedioctrl);
            //            }
            //            else
            //            {
            //                for (int i = 0; i < wrapPanel.Children.Count; ++i)
            //                {
            //                    vedioCtrl vedioctrlTemp = wrapPanel.Children[i] as vedioCtrl;
            //                    int compare = ipstring.CompareTo((vedioctrlTemp.btnText.Content as String));
            //                    if (compare == 0)
            //                    {   ////如果刚好找到设备
            //                        vedioctrlTemp._time         = DateTime.Now;
            //                        break;
            //                    }
            //                    else if (compare < 0)
            //                    {   ////如果设备在当前设备IP字符串的前面
            //                        vedioCtrl vedioctrl         = new vedioCtrl();
            //                        vedioctrl.Width             = 100;
            //                        vedioctrl.Height            = 100;
            //                        vedioctrl._time             = DateTime.Now;
            //                        vedioctrl.MouseDoubleClick  +=vedioctrl_MouseDoubleClick;
            //                        vedioctrl.btnText.Content   = ipstring;
            //                        wrapPanel.Children.Insert(i, vedioctrl);
            //                        break;
            //                    }
            //                }
            //            }
            //        }
            //       ));
            //    }
            //}

            //u.BeginReceive(new AsyncCallback(ReceiveCallback), u);
        }

        void vedioctrl_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            try
            {
                vedioCtrl vedioctrl = sender as vedioCtrl;
                if (vedioctrl != null && _StartConnect != null)
                {
                    _StartConnect(vedioctrl.btnText.Content as string);
                }
            }
            catch
            {

            }
        }

        public IPAddress[] getBroadcasetAddress()
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

        void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            int ii = Thread.CurrentThread.ManagedThreadId;
            _udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), _udpClient);

            //Thread tRemove = new Thread(() =>
            //{   ////循环查询过期IP并删除之
            //    while (true)
            //    {
            //        try
            //        {
            //            this.Dispatcher.BeginInvoke(new Action(() =>
            //            {
            //                for (int i = 0; i < wrapPanel.Children.Count; ++i)
            //                {
            //                    vedioCtrl vedioctrlTemp = wrapPanel.Children[i] as vedioCtrl;
            //                    if ((DateTime.Now - vedioctrlTemp._time).TotalSeconds >= 25)
            //                    {
            //                        wrapPanel.Children.Remove(vedioctrlTemp);
            //                        i--;
            //                    }
            //                }
            //            }
            //         ));
            //            Thread.Sleep(10*1000);
            //        }
            //        catch
            //        {

            //        }
            //    }
            //}
            //);
            //tRemove.IsBackground = true;
            //tRemove.Start();

            //Thread tBroad = new Thread(() =>
            //{   ////循环定期广播发送服务在线
            //    while (true)
            //    {
            //        try
            //        {
            //            System.Net.IPAddress[] addressList = getBroadcasetAddress();

            //            Socket      sock    = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);//初始化一个Scoket实习,采用UDP传输
            //            sock.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);//设置该scoket实例的发送形式
            //            string      request = "uyouserver=online";//初始化需要发送而的发送数据
            //            byte[]      buffer  = Encoding.ASCII.GetBytes(request);
            //            foreach(IPAddress ip in addressList)
            //            {
            //                sock.SendTo(buffer, new IPEndPoint(ip, _udpPort));
            //            }
            //            sock.Close();

            //            Thread.Sleep(10*1000);
            //        }
            //        catch
            //        {

            //        }
            //    }
            //}
            //);
            //tBroad.IsBackground = true;
            //tBroad.Start();

            //for (int i  = 0; i < 100; ++i)
            //{
            //    vedioCtrl vedioctrl         = new vedioCtrl();
            //    vedioctrl.Width             = 100;
            //    vedioctrl.Height            = 100;
            //    vedioctrl._time             = DateTime.Now;
            //    vedioctrl.MouseDoubleClick  +=vedioctrl_MouseDoubleClick;
            //    vedioctrl.btnText.Content   = i.ToString();
            //    wrapPanel.Children.Insert(i, vedioctrl);

            //    //IPAddress ipaddr = IPAddress.Parse("127.0.0.1");
            //    //TcpListener listener = new TcpListener(ipaddr, 2122);

            //    //TcpClient client = listener.AcceptTcpClient();
            //    //NetworkStream stream =  client.GetStream();
            //    //StreamReader  readStream = new StreamReader(stream);

            //    //readStream.ReadAsync();
            //}
        }
    }
}
