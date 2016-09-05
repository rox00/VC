using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using UYouMain.Common;

namespace UYouMain.ViewModel
{
    public class IntToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            Key key = KeyInterop.KeyFromVirtualKey((int)value);

            if (key == Key.None)
                return "";
            else
                return (key.ToString());
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return new NotImplementedException();
        }
    }

    class KeyMapCtrCanvas : NotificationObject
    {
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
        public KeyMapCtrCanvas()
        {
            windowLeft          = 0;
            windowTop           = 0;
            windowWidth         = 0;
            windowHeight        = 0;
        }

    }
    class KeyMapCtrImage : NotificationObject
    {

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
        private string                          source;
        public string Source
        {
            get { return source; }
            set
            {
                source = value;
                this.RaisePropertyChanged("Source");
            }
        }
        /****************************************************/

        public DelegateCommand<object> DeleteCommand { get; set; }
        public KeyMapCtrImage()
        {
            windowLeft          = 0;
            windowTop           = 0;
            windowWidth         = 0;
            windowHeight        = 0;
        }


    }
    class KeyMapCtrButton : NotificationObject
    {

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
        public Key valueProperty;
        public Key ValueProperty
        {
            get { return valueProperty; }
            set
            {
                valueProperty = value;
                this.RaisePropertyChanged("ValueProperty");
            }
        }
        /****************************************************/
        public DelegateCommand<object> DeleteCommand { get; set; }
        public KeyMapCtrButton()
        {
            windowLeft          = 0;
            windowTop           = 0;
            windowWidth         = 0;
            windowHeight        = 0;

            ValueProperty       = Key.None;
        }


    }
    class KeyMapCtrDirection : NotificationObject
    {
        /****************************************************/
        private int up          = KeyInterop.VirtualKeyFromKey(Key.W);
        private int left        = KeyInterop.VirtualKeyFromKey(Key.A);
        private int right       = KeyInterop.VirtualKeyFromKey(Key.S);
        private int down        = KeyInterop.VirtualKeyFromKey(Key.D);
        public int Up
        {
            get { return up; }
            set { up = value; }
        }

        public int Left
        {
            get { return left; }
            set { left = value; }
        }

        public int Right
        {
            get { return right; }
            set { right = value; }
        }

        public int Down
        {
            get { return down; }
            set { down = value; }
        }
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

        public DelegateCommand<object> DeleteCommand { get; set; }
        public KeyMapCtrDirection()
        {
            windowLeft          = 0;
            windowTop           = 0;
            windowWidth         = 0;
            windowHeight        = 0;

        }
    }
    public class KeyMapCtrlTemplateSelector : DataTemplateSelector
    {
        public DataTemplate CanvasTemplate { get; set; }
        public DataTemplate ImageTemplate { get; set; }
        public DataTemplate ButtonTemplate { get; set; }
        public DataTemplate DirectionTemplate { get; set; }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            FrameworkElement element = container as FrameworkElement;

            if (element != null && item != null)
            {
                if (item is KeyMapCtrCanvas)
                    return CanvasTemplate;
                if (item is KeyMapCtrImage)
                    return ImageTemplate;
                if (item is KeyMapCtrButton)
                    return ButtonTemplate;
                if (item is KeyMapCtrDirection)
                    return DirectionTemplate;
            }

            return null;
        }
    }

    class KeyMapModel : NotificationObject
    {
        /****************************************************/
        private ObservableCollection<object> keyCtrArray;
        public ObservableCollection<object> KeyCtrArray
        {
            get
            {
                return keyCtrArray;
            }
            set
            {
                keyCtrArray = value;
                this.RaisePropertyChanged("KeyCtrArray");
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

        public DelegateCommand<object> UpdateRadioCommand { get; set; }

        public Func<bool>                       SaveConfig;
        public Func<KeyMappingConfig>           getKeyMappingConfig;

        private KeyMappingConfig                _keyConfigTemp      = new KeyMappingConfig();   ////以1280*720的实际像素代替
        private string                          AppName = "";
        private double                          _radioX             = 1.0;
        private double                          _radioY             = 1.0;

        private static                          KeyMapCtrDirection  itemDirection       = null;
        private static                          KeyMapCtrImage      itemFire            = null;
        private static                          KeyMapCtrImage      itemAim             = null;
        private static                          KeyMapCtrButton     newUserBtnTemp       = null;
        public KeyMapModel()        
        {
            KeyCtrArray                         = new ObservableCollection<object>();
            wndVisible                          = Visibility.Visible;

            windowLeft                          = 0;
            windowTop                           = 0;
            windowWidth                         = 500;
            windowHeight                        = 500;


            UpdateRadioCommand                             = new DelegateCommand<object>(new Action<object>(this.UpdateRadio));
            ReLoadVedioArray();
        }

        public void UpdateRadio(object obj)
        {
            Window wnd = obj as Window;
            Point topleft       = wnd.PointToScreen(new Point(0, 0));
            Point botright      = wnd.PointToScreen(new Point(wnd.Width, wnd.Height));
            _radioX             = 1280/(botright.X - topleft.X);
            _radioY             = 720/(botright.Y - topleft.Y);
        }

        private void Delete(object obj)
        {

        }
        public void ReLoadVedioArray()
        {
            KeyCtrArray.Clear();
            KeyMapCtrCanvas canvasCtrl = new KeyMapCtrCanvas();

            KeyCtrArray.Add(canvasCtrl);

            //KeyMapCtrButton btn         = new KeyMapCtrButton();
            //btn.ValueProperty           = (Key)58;
            //btn.WindowWidth             = 24;
            //btn.WindowHeight            = 24;
            //btn.WindowLeft              = 100;
            //btn.WindowTop               = 100;
            //btn.DeleteCommand           = new DelegateCommand<object>(new Action<object>(this.Delete)); ;

            ////canvasMain.Children.Add(btn);
            ////userBtnList.Add(btn);
            //KeyCtrArray.Add(btn);
        }

        private void Uninitiazize()
        {
            for(int i = 0; i < keyCtrArray.Count; ++i)
            {
                object obj = keyCtrArray[i];

                if(obj is KeyMapCtrButton || obj is KeyMapCtrDirection)
                {
                    keyCtrArray.Remove(obj);
                }
            }

           

            _keyConfigTemp.Mapping.Clear();
            _keyConfigTemp = new KeyMappingConfig();
        }

        public bool LoadConfig(string appName)
        {
            Uninitiazize();
            AppName = appName;

            Window keyMapWindow = null;
            foreach (Window win in Application.Current.Windows)
            {
                if (win is KeyMapping)
                {
                    keyMapWindow = win; break;
                }
            }

            List<Type> knownTypes = new List<Type>();
            knownTypes.Add(typeof(Keys));
            DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(KeyMappingConfig), knownTypes);
            try
            {
                string path = System.Windows.Forms.Application.StartupPath;
                path        += Common.Common.configFilePath + "\\" + Common.Common.mappingConfigPath + "\\";
                path        += appName + Common.Common.configFileExt;

                using (FileStream stream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                {
                    _keyConfigTemp = (KeyMappingConfig)serializer.ReadObject(stream);
                    stream.Close();
                }

                /*当前窗口的像素坐标*/
                Point  ptTopLeftThis                                        = keyMapWindow.PointToScreen(new Point(0, 0));
                if (_keyConfigTemp.Direction != null)
                {
                    /*计算相对于当前窗口的坐标*/
                    Point pt = keyMapWindow.PointFromScreen(new Point(
                    ptTopLeftThis.X + _keyConfigTemp.Direction.Center[0]/_radioX,
                    ptTopLeftThis.Y + _keyConfigTemp.Direction.Center[1]/_radioY)
                    );

                    _keyConfigTemp.Direction.Center[0]                          = pt.X;
                    _keyConfigTemp.Direction.Center[1]                          = pt.Y;
                    for (int i = 0; i < _keyConfigTemp.Direction.Arrows.Count; i++)
                    {
                        /*计算相对于当前窗口的坐标*/
                        pt = keyMapWindow.PointFromScreen(new Point(
                        ptTopLeftThis.X + ((Keys)_keyConfigTemp.Direction.Arrows[i]).Position[0]/_radioX,
                        ptTopLeftThis.Y + ((Keys)_keyConfigTemp.Direction.Arrows[i]).Position[1]/_radioY)
                        );
                        ((Keys)_keyConfigTemp.Direction.Arrows[i]).Position[0]  = pt.X;
                        ((Keys)_keyConfigTemp.Direction.Arrows[i]).Position[1]  = pt.Y;
                    }
                }
                for (int i = 0; i < _keyConfigTemp.Mapping.Count; i++)
                {
                    if (((Keys)_keyConfigTemp.Mapping[i]) != null)
                    {
                        /*计算相对于当前窗口的坐标*/
                        Point pt = keyMapWindow.PointFromScreen(new Point(
                            ptTopLeftThis.X + ((Keys)_keyConfigTemp.Mapping[i]).Position[0]/_radioX,
                            ptTopLeftThis.Y + ((Keys)_keyConfigTemp.Mapping[i]).Position[1]/_radioY)
                            );
                        ((Keys)_keyConfigTemp.Mapping[i]).Position[0]           = pt.X;
                        ((Keys)_keyConfigTemp.Mapping[i]).Position[1]           = pt.Y;
                    }
                }
                LoadSetting();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return false;
            }

            return true;
        }

        private void LoadSetting()
        {
            try
            {
                //ContextMenu cm      = new System.Windows.Controls.ContextMenu();
                //MenuItem menuItem   = new MenuItem();
                //menuItem.Header     = "删除";
                //menuItem.Click      += menuItem_Click;
                //cm.Items.Add(menuItem);

                KeyMapCtrCanvas canv = keyCtrArray[0] as KeyMapCtrCanvas;
                double width = canv.WindowTop;
                if (_keyConfigTemp.Direction != null)
                {
                    double directBtnWidth           = ((Keys)_keyConfigTemp.Direction.Arrows[2]).Position[0] - ((Keys)_keyConfigTemp.Direction.Arrows[1]).Position[0];
                    double directBtnHeight          = ((Keys)_keyConfigTemp.Direction.Arrows[3]).Position[1] - ((Keys)_keyConfigTemp.Direction.Arrows[0]).Position[1];
                    if (itemDirection == null)
                    {
                        itemDirection               = new KeyMapCtrDirection();
                        itemDirection.WindowLeft    = _keyConfigTemp.Direction.Center[0] - directBtnWidth / 2;
                        itemDirection.WindowTop     = _keyConfigTemp.Direction.Center[1]  - directBtnHeight / 2;
                        itemDirection.WindowWidth   = directBtnWidth;
                        itemDirection.WindowHeight  = directBtnHeight;

                        itemDirection.DeleteCommand = new DelegateCommand<object>(new Action<object>(this.Delete)); ;

                        KeyCtrArray.Add(itemDirection);
                    }

                    if (_keyConfigTemp.Direction.Arrows != null)
                    {
                        itemDirection.Up      = (int)((Keys)_keyConfigTemp.Direction.Arrows[0]).Key;
                        itemDirection.Left    = (int)((Keys)_keyConfigTemp.Direction.Arrows[1]).Key;
                        itemDirection.Right   = (int)((Keys)_keyConfigTemp.Direction.Arrows[2]).Key;
                        itemDirection.Down    = (int)((Keys)_keyConfigTemp.Direction.Arrows[3]).Key;
                    }
                }

                if (_keyConfigTemp.Mapping != null)
                {
                    if (_keyConfigTemp.Mapping[0] != null)
                    {
                        if (itemFire == null)
                        {
                            itemFire = new KeyMapCtrImage();
                            itemFire.WindowWidth        = 39;
                            itemFire.WindowHeight       = 38;
                            itemFire.WindowLeft         = ((Keys)_keyConfigTemp.Mapping[0]).Position[0] - itemFire.WindowWidth / 2;
                            itemFire.WindowTop          = ((Keys)_keyConfigTemp.Mapping[0]).Position[1] - itemFire.WindowHeight / 2;
                            itemFire.Source             = "/UYouMain;component/Resources/imgs/black/四向2.png";

                            //itemFire.SetValue(NameProperty, fireName);
                            itemFire.DeleteCommand = new DelegateCommand<object>(new Action<object>(this.Delete)); ;

                            KeyCtrArray.Add(itemFire);
                        }
                    }

                    if (_keyConfigTemp.Mapping[1] != null)
                    {
                        if (itemAim == null)
                        {
                            itemAim = new KeyMapCtrImage();
                            itemAim.WindowWidth        = 39;
                            itemAim.WindowHeight       = 38;
                            itemAim.WindowLeft         = ((Keys)_keyConfigTemp.Mapping[1]).Position[0] - itemAim.WindowWidth / 2;
                            itemAim.WindowTop          = ((Keys)_keyConfigTemp.Mapping[1]).Position[1] - itemAim.WindowHeight / 2;
                            itemAim.Source             = "/UYouMain;component/Resources/imgs/black/四向2.png";
                            itemAim.DeleteCommand       = new DelegateCommand<object>(new Action<object>(this.Delete)); ;
                            //itemAim.SetValue(NameProperty, aimName);
                            //canvasMain.RegisterName(aimName, itemAim);
                            //canvasMain.Children.Add(itemAim);
                            KeyCtrArray.Add(itemAim);
                        }
                    }

                    //userBtnList.Clear();
                    for (int i = 2; i < _keyConfigTemp.Mapping.Count; i++)
                    {
                        KeyMapCtrButton btn         = new KeyMapCtrButton();
                        btn.ValueProperty           = (Key)Convert.ToUInt32(((Keys)_keyConfigTemp.Mapping[i]).Key);
                        btn.WindowWidth             = 49;
                        btn.WindowHeight            = 48;
                        btn.WindowLeft              = ((Keys)_keyConfigTemp.Mapping[i]).Position[0] - 24 / 2;
                        btn.WindowTop               = ((Keys)_keyConfigTemp.Mapping[i]).Position[1] - 24 / 2;
                        btn.DeleteCommand           = new DelegateCommand<object>(new Action<object>(this.Delete)); ;

                        //canvasMain.Children.Add(btn);
                        //userBtnList.Add(btn);
                        KeyCtrArray.Add(btn);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
        public void ExitWnd(object obj)
        {
            
        }
    }
}
