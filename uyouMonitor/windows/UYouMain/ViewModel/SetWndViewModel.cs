using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows;
using UYouMain.Common;

namespace UYouMain.ViewModel
{
    public class resolution
    {
        public string       text { get; set; }
        public bool         bSelect { get; set; }
    }
    public class SetWndViewModel : NotificationObject
    {
       public SetWndViewModel()
       {
           resolution1                          = new resolution();
           resolution2                          = new resolution();
           resolution3                          = new resolution();
           resolution4                          = new resolution();

           resolution1.text                     = "100%(1280*720)";
           resolution2.text                     = "75%";
           resolution3.text                     = "50%";
           resolution4.text                     = "25%";

           resolution1.bSelect                  = true;
           resolution2.bSelect                  = false;
           resolution3.bSelect                  = false;
           resolution4.bSelect                  = false;


           saveCommand                          = new DelegateCommand<object>(new Action<object>(this.Save));
           exitCommand                          = new DelegateCommand<object>(new Action<object>(this.Exit));

           mainWindowWidth                      = 0;
           mainWindowHeight                     = 0;
           using (Graphics graphics = Graphics.FromHwnd(IntPtr.Zero))
           {
               float    _dpiX                   = 0;
               float    _dpiY                   = 0;
               _dpiX = graphics.DpiX;
               _dpiY = graphics.DpiY;

               if(graphics.DpiX > 0)
                   mainWindowWidth              = 1280 * 96 / graphics.DpiX * 17 / 16;
               if (graphics.DpiY > 0)
                   mainWindowHeight             = 720 * 96 / graphics.DpiX  + 18;
           }

           Load(null);
       }

       public resolution                        resolution1 { get; set; }
       public resolution                        resolution2 { get; set; }
       public resolution                        resolution3 { get; set; }
       public resolution                        resolution4 { get; set; }

       public DelegateCommand<object>           saveCommand { get; set; }
       public DelegateCommand<object>           exitCommand { get; set; }

       private double                           mainWindowLeft;
       public double                            MainWindowLeft
       {
           get { return mainWindowLeft; }
           set
           {
               mainWindowLeft = value;
               this.RaisePropertyChanged("MainWindowLeft");
           }
       }

       private double                           mainWindowTop;
       public double                            MainWindowTop
       {
           get { return mainWindowTop; }
           set
           {
               mainWindowTop = value;
               this.RaisePropertyChanged("MainWindowTop");
           }
       }

       private double                           mainWindowWidth;
       public double                            MainWindowWidth
       {
           get { return mainWindowWidth; }
           set 
           { 
                mainWindowWidth = value; 
                this.RaisePropertyChanged("MainWindowWidth");
           }
       }

       private double                           mainWindowHeight;
       public double                            MainWindowHeight
       {
           get { return mainWindowHeight; }
           set 
           { 
                mainWindowHeight = value;
                this.RaisePropertyChanged("MainWindowHeight");
           }
       }
       

       public void Exit(object obj)
       {
           if (obj is Window)
           {
               Window wnd = obj as Window;
               wnd.Close();
           }
       }

       public void Save(object obj)
       {
           {////先保存 或删除 快捷启动方式
               //string lnkPath = System.Environment.GetFolderPath(Environment.SpecialFolder.CommonStartup) + "\\" 
               //+ System.IO.Path.GetFileName(Assembly.GetExecutingAssembly().Location) +".lnk";

               //if (bStartup == true)
               //{
               //    CreateLnk(lnkPath);
               //}
               //else
               //{
               //    if (System.IO.File.Exists(lnkPath))
               //        System.IO.File.Delete(lnkPath);
               //}

           }


           using (Graphics graphics = Graphics.FromHwnd(IntPtr.Zero))
           {
               Configuration config                 = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
               config.AppSettings.Settings.Remove("setwnd_resolutionIndex");

               if (resolution1.bSelect)
               {
                   if (graphics.DpiX > 0)
                       MainWindowWidth              = 1280 * 96 / graphics.DpiX * 17 / 16;
                   if (graphics.DpiY > 0)
                       MainWindowHeight             = 720 * 96 / graphics.DpiY  + 18;

                   config.AppSettings.Settings.Add("setwnd_resolutionIndex", "1");
               }
               if (resolution2.bSelect)
               {
                   if (graphics.DpiX > 0)
                       MainWindowWidth              = 1280 * 96 / graphics.DpiX * 0.75  * 17 / 16;
                   if (graphics.DpiY > 0)
                       MainWindowHeight             = 720 * 96 / graphics.DpiY * 0.75   + 18;

                   config.AppSettings.Settings.Add("setwnd_resolutionIndex", "2");
               }
               if (resolution3.bSelect)
               {
                   if (graphics.DpiX > 0)
                       MainWindowWidth              = 1280 * 96 / graphics.DpiX * 0.50  * 17 / 16;
                   if (graphics.DpiY > 0)
                       MainWindowHeight             = 720 * 96 / graphics.DpiY * 0.50   + 18;

                   config.AppSettings.Settings.Add("setwnd_resolutionIndex", "3");
               }
               if (resolution4.bSelect)
               {
                   if (graphics.DpiX > 0)
                       MainWindowWidth              = 1280 * 96 / graphics.DpiX * 0.25  * 17 / 16;
                   if (graphics.DpiY > 0)
                       MainWindowHeight             = 720 * 96 / graphics.DpiY * 0.25   + 18;

                   config.AppSettings.Settings.Add("setwnd_resolutionIndex", "4");
               }

               config.Save();
               ConfigurationManager.RefreshSection("appSettings");

               if (obj is Window)
               {
                   Window wnd = obj as Window;
                   wnd.Close();
               }
           }
       }

       private void Load(object obj)
       {
           {////查询是否有快捷启动方式
               //string lnkPath = System.Environment.GetFolderPath(Environment.SpecialFolder.Startup) + "\\" 
               //+ System.IO.Path.GetFileName(Assembly.GetExecutingAssembly().Location) +".lnk";

               //if (System.IO.File.Exists(lnkPath))
               //    bStartup = true;
               //else
               //    bStartup = false;

           }

           foreach (string key in ConfigurationManager.AppSettings)
           {    //检索当前选中的分辨率
               if (key == "setwnd_resolutionIndex")
               {
                   string value    = ConfigurationManager.AppSettings[key];
                   int intvalue    = 1;
                   Int32.TryParse(value, out intvalue);

                   resolution1.bSelect = false;
                   resolution2.bSelect = false;
                   resolution3.bSelect = false;
                   resolution4.bSelect = false;


                   using (Graphics graphics = Graphics.FromHwnd(IntPtr.Zero))
                   {
                       if (intvalue == 1)
                       {
                           resolution1.bSelect = true;

                           if (graphics.DpiX > 0)
                               mainWindowWidth              = 1280 * 96 / graphics.DpiX * 17 / 16;
                           if (graphics.DpiY > 0)
                               mainWindowHeight             = 720 * 96 / graphics.DpiY  + 18;
                       }
                       if (intvalue == 2)
                       {
                           resolution2.bSelect = true;

                           if (graphics.DpiX > 0)
                               mainWindowWidth              = 1280 * 96 / graphics.DpiX * 0.75  * 17 / 16;
                           if (graphics.DpiY > 0)
                               mainWindowHeight             = 720 * 96 / graphics.DpiY * 0.75   + 18;
                       }
                       if (intvalue == 3)
                       {
                           resolution3.bSelect = true;

                           if (graphics.DpiX > 0)
                               mainWindowWidth              = 1280 * 96 / graphics.DpiX * 0.50  * 17 / 16;
                           if (graphics.DpiY > 0)
                               mainWindowHeight             = 720 * 96 / graphics.DpiY * 0.50   + 18;
                       }
                       if (intvalue == 4)
                       {
                           resolution4.bSelect = true;

                           if (graphics.DpiX > 0)
                               mainWindowWidth              = 1280 * 96 / graphics.DpiX * 0.25  * 17 / 16;
                           if (graphics.DpiY > 0)
                               mainWindowHeight             = 720 * 96 / graphics.DpiY * 0.25   + 18;
                       }
                   }
               }
           }
       }
       
       private void CreateLnk(string lnkPath)
       {
           if (!System.IO.File.Exists(lnkPath))
           {
               IWshRuntimeLibrary.WshShell shell           = new IWshRuntimeLibrary.WshShell();
               IWshRuntimeLibrary.IWshShortcut shortCut    = (IWshRuntimeLibrary.IWshShortcut)shell.CreateShortcut(lnkPath);
               shortCut.TargetPath                         = Assembly.GetExecutingAssembly().Location;
               shortCut.WindowStyle                        = 1;
               shortCut.Description                        = Assembly.GetExecutingAssembly().FullName;
               shortCut.IconLocation                       = Assembly.GetExecutingAssembly().Location;
               shortCut.WorkingDirectory                   = System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
               shortCut.Save();
           }
       }
    }
}
