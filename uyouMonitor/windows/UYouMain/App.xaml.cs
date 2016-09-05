using System;
using System.IO;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Threading;

namespace UYouMain
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        //System.Threading.Mutex mutex;  


        private void Application_Startup(object sender, StartupEventArgs e)
        {
            //bool ret;
            //mutex = new System.Threading.Mutex(true, "U游播放器", out ret);

            //if (!ret)
            //{
            //    Environment.Exit(0);  
            //}
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
        }

        void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Guid guid = Guid.NewGuid();
            string dtNow = DateTime.Now.ToString("yyyy_MM_dd");
            string file = dtNow + "_" + guid + ".dmp";
            MiniDumpUtil.TryWriteMiniDump(System.Windows.Forms.Application.StartupPath + "\\" + file, 
                MiniDumpType.MiniDumpWithFullMemory);

        }
    }
}
