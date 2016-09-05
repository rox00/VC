using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;   
using System.Runtime.Serialization.Json;
using System.IO;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Windows.Input;

namespace UYouMain.Common
{    
    public class Common
    {
        public static NLog.Logger   log                 = NLog.LogManager.GetCurrentClassLogger();
        public static string        configFilePath      = "\\User";
        public static string        mappingConfigPath   = "\\mapping";
        public static string        configFileExt       = ".cfg";
        public static string        configFile          = "config";
        public static string        updataConfigFile    = "\\upgrade.cfg";
        public static string        updataFilePath      = "\\download";

        public static Size          boxSize             = new Size(1280, 720);

        public static byte[] StructToBytes(object obj)
        {
            int size = Marshal.SizeOf(obj);
            byte[] bytes = new byte[size];
            IntPtr structPtr = Marshal.AllocHGlobal(size); //分配结构体大小的内存空间
            Marshal.StructureToPtr(obj, structPtr, false); //将结构体拷到分配好的内存空间
            Marshal.Copy(structPtr, bytes, 0, size);       //从内存空间拷到byte数组
            Marshal.FreeHGlobal(structPtr);                //释放内存空间
            return bytes;
        }

        public static object BytesToStruct(byte[] bytes, Type type)
        {
            int size = Marshal.SizeOf(type);
            if (size > bytes.Length)
                return null;
            IntPtr structPtr = Marshal.AllocHGlobal(size); //分配结构大小的内存空间
            Marshal.Copy(bytes, 0, structPtr, size);       //将byte数组拷到分配好的内存空间
            object obj = Marshal.PtrToStructure(structPtr, type);
            Marshal.FreeHGlobal(structPtr);
            return obj;
        }
    }

    public class NotificationObject : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public void RaisePropertyChanged(string propertyName)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }

    /// <summary> 
    /// delegate command for view model 
    /// </summary> 
    public class DelegateCommand<T> : ICommand
    {
        #region members
        /// <summary> 
        /// can execute function 
        /// </summary> 
        private readonly Func<T, bool> canExecute;

        /// <summary> 
        /// execute function 
        /// </summary> 
        private readonly Action<T> execute;

        #endregion

        /// <summary> 
        /// Initializes a new instance of the DelegateCommand class. 
        /// </summary> 
        /// <param name="execute">indicate an execute function</param> 
        public DelegateCommand(Action<T> execute)
            : this(execute, null)
        {
        }

        /// <summary> 
        /// Initializes a new instance of the DelegateCommand class. 
        /// </summary> 
        /// <param name="execute">execute function </param> 
        /// <param name="canExecute">can execute function</param> 
        public DelegateCommand(Action<T> execute, Func<T, bool> canExecute)
        {
            this.execute = execute;
            this.canExecute = canExecute;
        }

        /// <summary> 
        /// can executes event handler 
        /// </summary> 
        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        /// <summary> 
        /// implement of icommand can execute method 
        /// </summary> 
        /// <param name="parameter">parameter by default of icomand interface</param> 
        /// <returns>can execute or not</returns> 
        public bool CanExecute(object parameter)
        {
            if (this.canExecute == null)
            {
                return true;
            }

            return this.canExecute((T)parameter);
        }

        /// <summary> 
        /// implement of icommand interface execute method 
        /// </summary> 
        /// <param name="parameter">parameter by default of icomand interface</param> 
        public void Execute(object parameter)
        {
            this.execute((T)parameter);
        }
    }

    enum BoxFlags
    {
        BoxFlags_IsOpenDeviceControl = 1,
        BoxFlags_IsHorizontalMode = 2,
    };

    public enum HitTest : int
    {
        HTERROR         = -2,
        HTTRANSPARENT   = -1,
        HTNOWHERE       = 0,
        HTCLIENT        = 1,
        HTCAPTION       = 2,
        HTSYSMENU       = 3,
        HTGROWBOX       = 4,
        HTSIZE          = HTGROWBOX,
        HTMENU          = 5,
        HTHSCROLL       = 6,
        HTVSCROLL       = 7,
        HTMINBUTTON     = 8,
        HTMAXBUTTON     = 9,
        HTLEFT          = 10,
        HTRIGHT         = 11,
        HTTOP           = 12,
        HTTOPLEFT       = 13,
        HTTOPRIGHT      = 14,
        HTBOTTOM        = 15,
        HTBOTTOMLEFT    = 16,
        HTBOTTOMRIGHT   = 17,
        HTBORDER        = 18,
        HTREDUCE        = HTMINBUTTON,
        HTZOOM          = HTMAXBUTTON,
        HTSIZEFIRST     = HTLEFT,
        HTSIZELAST      = HTBOTTOMRIGHT,
        HTOBJECT        = 19,
        HTCLOSE         = 20,
        HTHELP          = 21,
    }

    public class Win32APIs
    {
        [System.Runtime.InteropServices.StructLayoutAttribute(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct tagCOPYDATASTRUCT
        {

            /// ULONG_PTR->unsigned int
            public uint dwData;

            /// DWORD->unsigned int
            public uint cbData;

            /// PVOID->void*
            public System.IntPtr lpData;
        }

        /// <summary>
        /// 带有外边框和标题的windows的样式
        /// </summary>
        public const int WS_CAPTION = 0X00C0000;

        /// <summary>
        /// 系统菜单
        /// </summary>
        public const int WS_SYSMENU = 0x00080000;

        /// <summary>
        /// window 扩展样式 分层显示
        /// </summary>
        public const int WS_EX_LAYERED = 0x00080000;

        /// <summary>
        /// 带有alpha的样式
        /// </summary>
        public const int LWA_ALPHA = 0x00000002;

        /// <summary>
        /// 颜色设置
        /// </summary>
        public const int LWA_COLORKEY = 0x00000001;

        /// <summary>
        /// window的基本样式
        /// </summary>
        public const int GWL_STYLE = -16;

        /// <summary>
        /// window的扩展样式
        /// </summary>
        public const int GWL_EXSTYLE = -20;

        /// <summary>
        /// 设置窗体的样式
        /// </summary>
        /// <param name="handle">操作窗体的句柄</param>
        /// <param name="oldStyle">进行设置窗体的样式类型.</param>
        /// <param name="newStyle">新样式</param>
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        public static extern void SetWindowLong(IntPtr handle, int oldStyle, int newStyle);

        /// <summary>
        /// 获取窗体指定的样式.
        /// </summary>
        /// <param name="handle">操作窗体的句柄</param>
        /// <param name="style">要进行返回的样式</param>
        /// <returns>当前window的样式</returns>
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        public static extern int GetWindowLong(IntPtr handle, int style);

        /// <summary>
        /// 设置窗体的工作区域.
        /// </summary>
        /// <param name="handle">操作窗体的句柄.</param>
        /// <param name="handleRegion">操作窗体区域的句柄.</param>
        /// <param name="regraw">if set to <c>true</c> [regraw].</param>
        /// <returns>返回值</returns>
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        public static extern int SetWindowRgn(IntPtr handle, IntPtr handleRegion, bool regraw);

        /// <summary>
        /// 创建带有圆角的区域.
        /// </summary>
        /// <param name="x1">左上角坐标的X值.</param>
        /// <param name="y1">左上角坐标的Y值.</param>
        /// <param name="x2">右下角坐标的X值.</param>
        /// <param name="y2">右下角坐标的Y值.</param>
        /// <param name="width">圆角椭圆的 width.</param>
        /// <param name="height">圆角椭圆的 height.</param>
        /// <returns>hRgn的句柄</returns>
        [System.Runtime.InteropServices.DllImport("gdi32.dll")]
        public static extern IntPtr CreateRoundRectRgn(int x1, int y1, int x2, int y2, int width, int height);

        /// <summary>
        /// Sets the layered window attributes.
        /// </summary>
        /// <param name="handle">要进行操作的窗口句柄</param>
        /// <param name="colorKey">RGB的值</param>
        /// <param name="alpha">Alpha的值，透明度</param>
        /// <param name="flags">附带参数</param>
        /// <returns>true or false</returns>
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        public static extern bool SetLayeredWindowAttributes(IntPtr handle, uint colorKey, byte alpha, int flags);

        [System.Runtime.InteropServices.DllImportAttribute("user32.dll", EntryPoint = "RegisterHotKey")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.Bool)]
        public static extern bool RegisterHotKey([System.Runtime.InteropServices.InAttribute()] System.IntPtr hWnd, int id, uint fsModifiers, uint vk);

        [System.Runtime.InteropServices.DllImportAttribute("user32.dll", EntryPoint = "UnregisterHotKey")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.Bool)]
        public static extern bool UnregisterHotKey([System.Runtime.InteropServices.InAttribute()] System.IntPtr hWnd, int id);


    }
}






        //private void axSVCBox_NotifyCopyData(object sender, AxSVCBoxLib._DSVCBoxEvents_NotifyCopyDataEvent e)
        //{
        //    try
        //    {
        //        if (e.hWnd == 1)
        //        {

        //            Common.Win32APIs.tagCOPYDATASTRUCT copyData = (Common.Win32APIs.tagCOPYDATASTRUCT)Marshal.PtrToStructure(new IntPtr(e.pCopyDataStruct), typeof(Common.Win32APIs.tagCOPYDATASTRUCT));
        //            String str = Marshal.PtrToStringAuto(copyData.lpData);
        //            str = str.Replace("testindex=", "");
        //            int deviceNums = -1;
        //            Int32.TryParse(str, out deviceNums);

        //            RenderTargetBitmap bmp = new RenderTargetBitmap((int)this.ActualWidth,
        //                                                            (int)this.ActualHeight,
        //                                                            _dpiX,
        //                                                            _dpiY,
        //                                                            PixelFormats.Pbgra32);
        //            bmp.Render(this);

        //            //FileStream ms = File.Open("1.bmp", FileMode.Create);
        //            //PngBitmapEncoder encode = new PngBitmapEncoder();
        //            //encode.Frames.Add(BitmapFrame.Create(bmp));             
        //            //encode.Save(ms);
        //            //ms.Close();

        //            Int32 ColorValue    = -1;
        //            Array pixels        = Array.CreateInstance(typeof(Int32), 10);
        //            foreach (System.Windows.Point pt in VedioSamplePointList)
        //            {
        //                bmp.CopyPixels(new Int32Rect((int)pt.X, (int)pt.Y, 1, 1), pixels, (int)this.ActualWidth * 4, 0);
        //                if (pixels.GetLength(0) == 0)
        //                    break;
        //                System.Drawing.Color clr = System.Drawing.Color.FromArgb((int)pixels.GetValue(0));
        //                if (Math.Abs(clr.R - clr.G) >= 10 || Math.Abs(clr.B - clr.G) >= 10 || Math.Abs(clr.B - clr.R) >= 10)	//三分量必须全相等
        //                    break;

        //                Int32 GotColorValue = (clr.R + clr.G + clr.B) / 3;

        //                if (ColorValue == -1)
        //                    ColorValue = GotColorValue;
        //                else if (GotColorValue != ColorValue)		//测试点的颜色如果不一样，认为失败
        //                    break;
        //            }

        //            Int32 nIndex = ColorValue / 10 - 1;		//除以10以后反向计算出index

        //            if (ColorValue == -1 || nIndex != deviceNums)
        //            { //失败了，准备重试
        //                e.bResult = 0;
        //            }
        //            else
        //            {
        //                e.bResult = 1;
        //            }

        //        }

        //    }
        //    catch
        //    { }
        //}