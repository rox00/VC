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
using WpfDragAndDropSmorgasbord.DragDropFramework;
using WpfDragAndDropSmorgasbord.DragDropFrameworkData;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;
using System.Windows.Threading;
using System.Collections.ObjectModel;

namespace UYouMain
{
    /// <summary>
    /// KeyMapping.xaml 的交互逻辑
    /// </summary>
    public partial class KeyMapping : Window
    {
        private static Image            itemFire            = null;
        private static Image            itemAim             = null;
        private static DirectionButton  itemDirection       = null;
        private static String           fireName            = "itemFire";
        private static String           aimName             = "itemAim";
        private static String           directionName       = "itemDirection";

        private double                  directionBtnWidth   = 117;
        private double                  directionBtnHeight  = 123;

        private KeyMappingConfig        _keyConfigTemp      = new KeyMappingConfig();   ////以1280*720的实际像素代替

        private double                  _radioX             = 1.0;
        private double                  _radioY             = 1.0;

        private UserButton              newUserBtnTemp      = null;

        //private String                  _appName            = "";
        public String AppName
        {
            get { return (String)this.GetValue(AppNameProperty); }
            set { this.SetValue(AppNameProperty, value); }
        }

        public static readonly DependencyProperty AppNameProperty =
            DependencyProperty.Register("AppName", typeof(String), typeof(KeyMapping),
            new FrameworkPropertyMetadata(null, new PropertyChangedCallback(AppNamePropertyChangedCallback)));
        private static void AppNamePropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (sender != null && sender is KeyMapping)
            {
                KeyMapping clock = sender as KeyMapping;
                clock.OnAppNamePropertyChanged((String)arg.OldValue, (String)arg.NewValue);

            }
        }
        protected virtual void OnAppNamePropertyChanged(String oldValue, String newValue)
        {
            RoutedPropertyChangedEventArgs<String> arg =
                new RoutedPropertyChangedEventArgs<String>(oldValue, newValue, AppNamePropertyEvent);
            this.RaiseEvent(arg);

        }
        public static readonly RoutedEvent AppNamePropertyEvent =
            EventManager.RegisterRoutedEvent("AppNamePropertyChanged",
             RoutingStrategy.Bubble, typeof(RoutedPropertyChangedEventHandler<String>), typeof(KeyMapping));

        public event RoutedPropertyChangedEventHandler<String> AppNamePropertyChanged
        {
            add
            {
                this.AddHandler(AppNamePropertyEvent, value);
            }
            remove
            {
                this.RemoveHandler(AppNamePropertyEvent, value);
            }
        }

        NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();


        public KeyMapping()
        {
            InitializeComponent();
            Initialize();

            this.PreviewMouseDown   += KeyMapping_PreviewMouseDown;
            this.PreviewKeyUp       += KeyMapping_PreviewKeyUp;
            this.SizeChanged        += KeyMapping_SizeChanged;
        }

        /*********************自定义函数*****************************/
        public KeyMappingConfig getKeyMappingConfig()
        {
            return _keyConfigTemp;
        }
        private void LoadSetting()
        {
            try
            {
                ContextMenu cm      = new System.Windows.Controls.ContextMenu();
                MenuItem menuItem   = new MenuItem();
                menuItem.Header     = "删除";
                menuItem.Click      += menuItem_Click;
                cm.Items.Add(menuItem);

                if (_keyConfigTemp.Direction != null)
                {
                    double directBtnWidth           = ((Keys)_keyConfigTemp.Direction.Arrows[2]).Position[0] - ((Keys)_keyConfigTemp.Direction.Arrows[1]).Position[0];
                    double directBtnHeight          = ((Keys)_keyConfigTemp.Direction.Arrows[3]).Position[1] - ((Keys)_keyConfigTemp.Direction.Arrows[0]).Position[1];
                    if (itemDirection == null)
                    {
                        itemDirection               = new DirectionButton();
                        itemDirection.Width         = directBtnWidth;
                        itemDirection.Height        = directBtnHeight;
                        itemDirection.SetValue(NameProperty, directionName);

                        itemDirection.menuDel_direction.Click +=menuDel_direction_Click;

                        canvasMain.Children.Add(itemDirection);
                        canvasMain.RegisterName(directionName, itemDirection);
                    }

                    itemDirection.SetValue(Canvas.LeftProperty, _keyConfigTemp.Direction.Center[0] - directBtnWidth / 2);
                    itemDirection.SetValue(Canvas.TopProperty, _keyConfigTemp.Direction.Center[1]  - directBtnHeight / 2);

                    DirectionButton.DirectionKey key = new DirectionButton.DirectionKey();
                    if (_keyConfigTemp.Direction.Arrows != null)
                    {
                        key.Up      = (int)((Keys)_keyConfigTemp.Direction.Arrows[0]).Key;
                        key.Left    = (int)((Keys)_keyConfigTemp.Direction.Arrows[1]).Key;
                        key.Right   = (int)((Keys)_keyConfigTemp.Direction.Arrows[2]).Key;
                        key.Down    = (int)((Keys)_keyConfigTemp.Direction.Arrows[3]).Key;
                        itemDirection.directionKeyProperty  = key;
                    }
                }

                if (_keyConfigTemp.Mapping != null)
                {
                    if (_keyConfigTemp.Mapping[0] != null)
                    {
                        if (itemFire == null)
                        {
                            itemFire = new Image();
                            itemFire.SetValue(Image.SourceProperty, imgFire.Source);
                            itemFire.SetValue(NameProperty, fireName);
                            itemFire.SetValue(Image.WidthProperty, imgFire.Width);
                            itemFire.SetValue(Image.HeightProperty, imgFire.Height);
                            itemFire.SetValue(ContextMenuProperty, cm);
                            canvasMain.RegisterName(fireName, itemFire);
                            canvasMain.Children.Add(itemFire);
                        }
                        itemFire.SetValue(Canvas.LeftProperty, ((Keys)_keyConfigTemp.Mapping[0]).Position[0] - imgFire.Width / 2);
                        itemFire.SetValue(Canvas.TopProperty, ((Keys)_keyConfigTemp.Mapping[0]).Position[1] - imgFire.Height / 2);

                    }

                    if (_keyConfigTemp.Mapping[1] != null)
                    {
                        if (itemAim == null)
                        {
                            itemAim = new Image();
                            itemAim.SetValue(Image.SourceProperty, imgAim.Source);
                            itemAim.SetValue(NameProperty, aimName);
                            itemAim.SetValue(Image.WidthProperty, imgAim.Width);
                            itemAim.SetValue(Image.HeightProperty, imgAim.Height);
                            itemAim.SetValue(ContextMenuProperty, cm);
                            canvasMain.RegisterName(aimName, itemAim);
                            canvasMain.Children.Add(itemAim);
                        }
                        itemAim.SetValue(Canvas.LeftProperty, ((Keys)_keyConfigTemp.Mapping[1]).Position[0] - imgAim.Width / 2);
                        itemAim.SetValue(Canvas.TopProperty, ((Keys)_keyConfigTemp.Mapping[1]).Position[1] - imgAim.Height / 2);

                    }

                    //userBtnList.Clear();
                    for (int i = 2; i < _keyConfigTemp.Mapping.Count; i++)
                    {
                        UserButton btn      = new UserButton();
                        btn.valueProperty   = (int)((Keys)_keyConfigTemp.Mapping[i]).Key;
                        btn.SetValue(Canvas.LeftProperty, ((Keys)_keyConfigTemp.Mapping[i]).Position[0] -24);
                        btn.SetValue(Canvas.TopProperty, ((Keys)_keyConfigTemp.Mapping[i]).Position[1] -24);
                        btn.menuDel.Click   +=menuDel_Click;
                        canvasMain.Children.Add(btn);
                        //userBtnList.Add(btn);

                    }
                }
            }
            catch (Exception ex)
            {
                log.Error(ex);
            }
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
                    Point pt = PointFromScreen(new Point(
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
                        Point pt = PointFromScreen(new Point(
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
                log.Error(ex.Message);
                return false;
            }

            return true;
        }
        public bool SaveConfig()
        {
            using (var mutex = new System.Threading.Mutex(false, "saveKeymapping"))
            {
                try
                {
                    List<Type> knownTypes           = new List<Type>();
                    knownTypes.Add(typeof(Keys));
                    DataContractJsonSerializer ser  = new DataContractJsonSerializer(typeof(KeyMappingConfig), knownTypes);
                    MemoryStream stream             = new MemoryStream();

                    /*当前窗口的像素坐标*/
                    Point  ptTopLeftThis                                = PointToScreen(new Point(0, 0));

                    if (_keyConfigTemp == null)
                    {
                        _keyConfigTemp = new KeyMappingConfig();
                    }

                    if (itemDirection != null)
                    {
                        if (_keyConfigTemp.Direction == null)
                        {
                            _keyConfigTemp.Direction = new Direction();
                        }

                        /*计算相对于当前窗口的像素坐标*/
                        Point  ptTopLeftDirection                               = itemDirection.PointToScreen(new Point(0, 0));
                        Point  ptBotRightDirection                              = itemDirection.PointToScreen(new Point(itemDirection.Width, itemDirection.Height));
                        ptTopLeftDirection.X                                    -= ptTopLeftThis.X;
                        ptTopLeftDirection.Y                                    -= ptTopLeftThis.Y;
                        ptBotRightDirection.X                                   -= ptTopLeftThis.X;
                        ptBotRightDirection.Y                                   -= ptTopLeftThis.Y;
                        /*转换成1280*720 坐标*/
                        ptTopLeftDirection.X                                    *= _radioX;
                        ptTopLeftDirection.Y                                    *= _radioY;
                        ptBotRightDirection.X                                   *= _radioX;
                        ptBotRightDirection.Y                                   *= _radioY;

                        _keyConfigTemp.Direction.Center[0]                      = (ptTopLeftDirection.X + ptBotRightDirection.X)/2;
                        _keyConfigTemp.Direction.Center[1]                      = (ptTopLeftDirection.Y + ptBotRightDirection.Y)/2;
                        DirectionButton.DirectionKey key                        = itemDirection.GetValue(DirectionButton.DirectionKeyProperty) as DirectionButton.DirectionKey;
                        ((Keys)_keyConfigTemp.Direction.Arrows[0]).Key          = key.Up;
                        ((Keys)_keyConfigTemp.Direction.Arrows[0]).Position[0]  = _keyConfigTemp.Direction.Center[0];
                        ((Keys)_keyConfigTemp.Direction.Arrows[0]).Position[1]  = ptTopLeftDirection.Y;

                        ((Keys)_keyConfigTemp.Direction.Arrows[1]).Key          = key.Left;
                        ((Keys)_keyConfigTemp.Direction.Arrows[1]).Position[0]  = ptTopLeftDirection.X;
                        ((Keys)_keyConfigTemp.Direction.Arrows[1]).Position[1]  = _keyConfigTemp.Direction.Center[1];

                        ((Keys)_keyConfigTemp.Direction.Arrows[2]).Key          = key.Right;
                        ((Keys)_keyConfigTemp.Direction.Arrows[2]).Position[0]  = ptBotRightDirection.X;
                        ((Keys)_keyConfigTemp.Direction.Arrows[2]).Position[1]  = _keyConfigTemp.Direction.Center[1];

                        ((Keys)_keyConfigTemp.Direction.Arrows[3]).Key          = key.Down;
                        ((Keys)_keyConfigTemp.Direction.Arrows[3]).Position[0]  = _keyConfigTemp.Direction.Center[0];
                        ((Keys)_keyConfigTemp.Direction.Arrows[3]).Position[1]  = ptBotRightDirection.Y;
                    }
                    else
                    {
                        _keyConfigTemp.Direction = null;
                    }
                    if (itemFire != null)
                    {
                        /*计算相对于当前窗口的像素坐标*/
                        Point  ptTopLeftBtn                                     = itemFire.PointToScreen(new Point(0, 0));
                        Point  ptBotRightBtn                                    = itemFire.PointToScreen(new Point(itemFire.Width, itemFire.Height));
                        ptTopLeftBtn.X                                          -= ptTopLeftThis.X;
                        ptTopLeftBtn.Y                                          -= ptTopLeftThis.Y;
                        ptBotRightBtn.X                                         -= ptTopLeftThis.X;
                        ptBotRightBtn.Y                                         -= ptTopLeftThis.Y;
                        /*转换成1280*720 坐标*/
                        ptTopLeftBtn.X                                          *= _radioX;
                        ptTopLeftBtn.Y                                          *= _radioY;
                        ptBotRightBtn.X                                         *= _radioX;
                        ptBotRightBtn.Y                                         *= _radioY;

                        if (_keyConfigTemp.Mapping[0] == null)
                        {
                            _keyConfigTemp.Mapping[0] = new Keys();
                        }
                        ((Keys)_keyConfigTemp.Mapping[0]).Key                   = (int)MouseButton.Left;
                        ((Keys)_keyConfigTemp.Mapping[0]).Position[0]           = (ptTopLeftBtn.X + ptBotRightBtn.X)/2;
                        ((Keys)_keyConfigTemp.Mapping[0]).Position[1]           = (ptTopLeftBtn.Y + ptBotRightBtn.Y)/2;
                    }
                    else
                    {
                        _keyConfigTemp.Mapping[0] = null;
                    }

                    if (itemAim != null)
                    {
                        /*计算相对于当前窗口的像素坐标*/
                        Point  ptTopLeftBtn                                     = itemAim.PointToScreen(new Point(0, 0));
                        Point  ptBotRightBtn                                    = itemAim.PointToScreen(new Point(itemAim.ActualWidth, itemAim.ActualHeight));
                        ptTopLeftBtn.X                                          -= ptTopLeftThis.X;
                        ptTopLeftBtn.Y                                          -= ptTopLeftThis.Y;
                        ptBotRightBtn.X                                         -= ptTopLeftThis.X;
                        ptBotRightBtn.Y                                         -= ptTopLeftThis.Y;
                        /*转换成1280*720 坐标*/
                        ptTopLeftBtn.X                                          *= _radioX;
                        ptTopLeftBtn.Y                                          *= _radioY;
                        ptBotRightBtn.X                                         *= _radioX;
                        ptBotRightBtn.Y                                         *= _radioY;

                        if (_keyConfigTemp.Mapping[1] == null)
                        {
                            _keyConfigTemp.Mapping[1] = new Keys();
                        }
                        ((Keys)_keyConfigTemp.Mapping[1]).Key = (int)MouseButton.Right;
                        ((Keys)_keyConfigTemp.Mapping[1]).Position[0] = (ptTopLeftBtn.X + ptBotRightBtn.X)/2;
                        ((Keys)_keyConfigTemp.Mapping[1]).Position[1] = (ptTopLeftBtn.Y + ptBotRightBtn.Y)/2;
                    }
                    else
                    {
                        _keyConfigTemp.Mapping[1] = null;
                    }
                    _keyConfigTemp.Mapping.RemoveRange(2, _keyConfigTemp.Mapping.Count - 2);

                    foreach (UIElement element in canvasMain.Children)
                    {
                        if (element is UserButton)
                        {
                            UserButton btn                                          = element as UserButton;
                            
                            /*计算相对于当前窗口的像素坐标*/
                            Point  ptTopLeftBtn                                     = btn.PointToScreen(new Point(0, 0));
                            Point  ptBotRightBtn                                    = btn.PointToScreen(new Point(btn.ActualWidth, btn.ActualHeight));
                            ptTopLeftBtn.X                                          -= ptTopLeftThis.X;
                            ptTopLeftBtn.Y                                          -= ptTopLeftThis.Y;
                            ptBotRightBtn.X                                         -= ptTopLeftThis.X;
                            ptBotRightBtn.Y                                         -= ptTopLeftThis.Y;
                            /*转换成1280*720 坐标*/
                            ptTopLeftBtn.X                                          *= _radioX;
                            ptTopLeftBtn.Y                                          *= _radioY;
                            ptBotRightBtn.X                                         *= _radioX;
                            ptBotRightBtn.Y                                         *= _radioY;

                            Keys keys   = new Keys();
                            keys.Key    = btn.valueProperty;
                            if (btn.valueProperty == KeyInterop.VirtualKeyFromKey(Key.None))
                            {
                                continue;
                            }
                            keys.Position[0] = (ptTopLeftBtn.X + ptBotRightBtn.X)/2;
                            keys.Position[1] = (ptTopLeftBtn.Y + ptBotRightBtn.Y)/2;
                            _keyConfigTemp.Mapping.Add(keys);
                        }
                    }

                    ser.WriteObject(stream, _keyConfigTemp);

                    //byte[] dataBytes    = new byte[stream.Length];
                    //stream.Position     = 0;
                    //stream.Read(dataBytes, 0, (int)stream.Length);
                    //string dataString   = Encoding.UTF8.GetString(dataBytes);
                    //log.Info("当前配置信息为: 应用:" + _appName + "映射" + dataString);

                    stream.Close();
                }
                catch (Exception ex)
                {
                    log.Error(ex.Message);
                    return false;
                }
            }
            return true;
        }
        private void Initialize()
        {
            #region C A N V A S DRAG
            // Data Providers/Consumers

            CanvasDataProvider<Canvas, Image> canvasImageDataProvider =
                new CanvasDataProvider<Canvas, Image>("CanvasImageObject");
            CanvasDataConsumer<Canvas, Image> canvasImageDataConsumer =
                new CanvasDataConsumer<Canvas, Image>(new string[] { "CanvasImageObject" },
                    this);
            CanvasDataProvider<Canvas, DirectionButton> canvasDirectionButtonDataProvider =
                new CanvasDataProvider<Canvas, DirectionButton>("CanvasDirectionButtonObject");
            CanvasDataConsumer<Canvas, DirectionButton> canvasDirectionButtonDataConsumer =
                new CanvasDataConsumer<Canvas, DirectionButton>(new string[] { "CanvasDirectionButtonObject" },
                    this);

            CanvasDataProvider<Canvas, UserButton> canvasUserButtonDataProvider =
                new CanvasDataProvider<Canvas, UserButton>("CanvasUserButtonObject");
            CanvasDataConsumer<Canvas, UserButton> canvasUserButtonDataConsumer =
                new CanvasDataConsumer<Canvas, UserButton>(new string[] { "CanvasUserButtonObject" },
                    this);


            // Drag Managers            
            DragManager dragHelpercanvasMain = new DragManager(canvasMain,
                new IDataProvider[] {
                    canvasImageDataProvider,
                    canvasDirectionButtonDataProvider,
                    canvasUserButtonDataProvider
                });

            // Drop Managers
            DropManager dropHelpercanvasMain = new DropManager(this.canvasMain,
                new IDataConsumer[] {
                    canvasImageDataConsumer,
                    canvasDirectionButtonDataConsumer,
                    canvasUserButtonDataConsumer
                });
            #endregion

            btnClear.Click      += btnClear_Click;
            btnClose.Click      += btnClose_Click;


            if (_keyConfigTemp == null)
            {
                _keyConfigTemp = new KeyMappingConfig();
            }
        }
        private void Uninitiazize()
        {
            if (canvasMain.FindName(directionName) != null)
            {
                canvasMain.Children.Remove(itemDirection);
                canvasMain.UnregisterName(directionName);
                itemDirection = null;
            }
            if (canvasMain.FindName(aimName) != null)
            {
                canvasMain.Children.Remove(itemAim);
                canvasMain.UnregisterName(aimName);
                itemAim = null;
            }

            if (canvasMain.FindName(fireName) != null)
            {
                canvasMain.Children.Remove(itemFire);
                canvasMain.UnregisterName(fireName);
                itemFire = null;
            }

            System.Collections.ArrayList listTemp = new System.Collections.ArrayList();
            foreach (UIElement element in canvasMain.Children)
            {
                if (element is UserButton)
                {
                    UserButton btn          = element as UserButton;
                    //btn.btn.IsChecked       = false;
                    listTemp.Add(btn);
                }
            }
            foreach (UserButton btn in listTemp)
            {
                canvasMain.Children.Remove(btn);
            }
            listTemp.Clear();
           
            if (newUserBtnTemp != null)
            {
                //newUserBtn.btn.IsChecked    = false;
                newUserBtnTemp                  = null;
            }

            itemFire                        = null;
            itemAim                         = null;

            _keyConfigTemp.Mapping.Clear();
            _keyConfigTemp = new KeyMappingConfig();
        }
        private void createUserButton(bool isChecked)
        {
            UserButton btn      = new UserButton(true);
            btn.menuDel.Click   += menuDel_Click;
            newUserBtnTemp          = btn;

            canvasMain.Children.Add(btn);
            btn.Focus();
        }
        /*********************其它控件相关事件*****************************/
        public void refushDragItem(String name, ref Canvas container, ref UIElement srcItem, ref UIElement item)
        {
            string itemXml = "";
            ContextMenu cm = new System.Windows.Controls.ContextMenu();
            MenuItem menuItem = new MenuItem();
            menuItem.Header = "删除";
            menuItem.Click += menuItem_Click;
            cm.Items.Add(menuItem);
            if (name == imgDirection.Name || name == directionName)
            {
                if (itemDirection == null)                                    
                {
                    itemDirection                           = new DirectionButton();
                    itemDirection.Width                     = directionBtnWidth;
                    itemDirection.Height                    = directionBtnHeight;
                    itemDirection.menuDel_direction.Click   += menuDel_direction_Click;                    
                    itemDirection.currentDirection          = "1";
                    itemDirection.SetValue(NameProperty, directionName);

                    container.RegisterName(directionName, itemDirection);
                    container.Children.Add(itemDirection);

                    System.Collections.ArrayList listTemp = new System.Collections.ArrayList();
                    foreach (UIElement element in canvasMain.Children)
                    {
                        if (element is UserButton)
                        {
                            UserButton btn          = element as UserButton;
                            //btn.btn.IsChecked       = false;
                            listTemp.Add(btn);
                        }
                    }
                    foreach (UserButton btn in listTemp)
                    {
                        if (itemDirection.directionKeyProperty.Up == btn.valueProperty
                            || itemDirection.directionKeyProperty.Left == btn.valueProperty
                            || itemDirection.directionKeyProperty.Right == btn.valueProperty
                            || itemDirection.directionKeyProperty.Down == btn.valueProperty)
                        {
       
                            container.Children.Remove(btn);
                        }
                    }
                    listTemp.Clear(); 
                }
                item = itemDirection;                                                                
            }
            else if (name == imgAim.Name || name == aimName)
            {
                if (itemAim == null)
                {
                    itemXml = System.Windows.Markup.XamlWriter.Save(srcItem);
                    itemAim = System.Windows.Markup.XamlReader.Parse(itemXml) as Image;                    
                    itemAim.SetValue(Image.DataContextProperty, MouseButton.Right + 1);
                    itemAim.SetValue(NameProperty, aimName);                    
                    itemAim.SetValue(ContextMenuProperty, cm);                    
                    container.RegisterName(aimName, itemAim);
                    container.Children.Add(itemAim);                    
                }
                item = itemAim;
            }
            else if (name == imgFire.Name || name == fireName)
            {
                if (itemFire == null)
                {
                    itemXml = System.Windows.Markup.XamlWriter.Save(srcItem);
                    itemFire = System.Windows.Markup.XamlReader.Parse(itemXml) as Image;
                    itemFire.SetValue(NameProperty, fireName);                    
                    itemFire.SetValue(Image.DataContextProperty, MouseButton.Left + 1);
                    itemFire.SetValue(ContextMenuProperty, cm);
                    container.RegisterName(fireName, itemFire);
                    container.Children.Add(itemFire);
                }
                item = itemFire;
            }
            else if (container.Children.Contains(srcItem))
            {
                item = srcItem;
            }
        }
        private void menuItem_Click(object sender, RoutedEventArgs e)
        {
            var mi = sender as MenuItem;
            var cm = mi.Parent as ContextMenu;
            var parent = cm.PlacementTarget;
            if (parent is Image)
            {
                Image img = parent as Image;
                Canvas canvas = img.Parent as Canvas;
                if (img.Name == aimName && itemAim != null)
                {
                    canvas.Children.Remove(itemAim);
                    itemAim = null;
                    canvas.UnregisterName(aimName);
                }
                else if (img.Name == fireName && itemFire != null)
                {
                    canvas.Children.Remove(itemFire);
                    itemFire = null;
                    canvas.UnregisterName(fireName);
                }
            }
        }
        private void menuDel_direction_Click(object sender, RoutedEventArgs e)
        {
            var mi = sender as MenuItem;
            var cm = mi.Parent as ContextMenu;
            var grid = cm.PlacementTarget as Grid;
            if (grid.Parent is DirectionButton)
            {
                DirectionButton tmp = grid.Parent as DirectionButton;
                Canvas ppp = tmp.Parent as Canvas;
                ppp.Children.Remove(itemDirection);
                ppp.UnregisterName(directionName);
                itemDirection = null;
            }
        }
        private void btnClear_Click(object sender, RoutedEventArgs e)
        {
            Uninitiazize();
        }
        private void KeyMapping_PreviewKeyUp(object sender, KeyEventArgs e)
        {
            Key key = (e.Key == Key.System ? e.SystemKey : e.Key);
            if (e.Key == Key.System)
            {
                return;
            }
            switch (key)
            {
                case Key.Escape:
                case Key.Tab:
                case Key.CapsLock:
                case Key.LeftCtrl:
                case Key.RightCtrl:
                case Key.LeftShift:
                case Key.RightShift:
                case Key.LeftAlt:
                case Key.RightAlt:
                case Key.LWin:
                case Key.RWin:
                case Key.Back:
                case Key.Return:
                case Key.PrintScreen:
                case Key.Insert:
                case Key.Home:
                case Key.End:
                case Key.PageDown:
                case Key.PageUp:
                case Key.Pause:
                case Key.Delete:
                case Key.Left:
                case Key.Right:
                case Key.Up:
                case Key.Down:
                case Key.NumLock:
                    return;                    
                default:
                    break;
            }

            if (e.Source is DirectionButton)
            {
                System.Collections.ArrayList listTemp = new System.Collections.ArrayList();
                foreach (UIElement element in canvasMain.Children)
                {
                    if (element is UserButton)
                    {
                        UserButton btn          = element as UserButton;
                        listTemp.Add(btn);
                    }
                }
                foreach (UserButton btn in listTemp)
                {
                    if (btn.valueProperty == KeyInterop.VirtualKeyFromKey(key))
                    {
                        canvasMain.Children.Remove(btn);
                    }
                }
                listTemp.Clear(); 
            }
            else
            {
                if (itemDirection != null)
                {
                    if (KeyInterop.VirtualKeyFromKey(key) == itemDirection.directionKeyProperty.Up
                        || KeyInterop.VirtualKeyFromKey(key) == itemDirection.directionKeyProperty.Left
                        || KeyInterop.VirtualKeyFromKey(key) == itemDirection.directionKeyProperty.Right
                        || KeyInterop.VirtualKeyFromKey(key) == itemDirection.directionKeyProperty.Down)
                    {
                        return;
                    }
                }

                foreach (UIElement element in canvasMain.Children)
                {
                    if (element is UserButton)
                    {
                        UserButton btn          = element as UserButton;
                        if (btn.valueProperty == KeyInterop.VirtualKeyFromKey(key))
                        {
                            return;
                        }
                        if (btn.textbox.IsFocused)
                        {   ////如果当前有未赋值的按键，则将其当成新按键
                            newUserBtnTemp = btn;
                        }
                    }
                }
               
                if (newUserBtnTemp != null)
                {
                    if (newUserBtnTemp.valueProperty != KeyInterop.VirtualKeyFromKey(key))
                    {   ////如果未赋值的按键，则给其赋值
                        newUserBtnTemp.valueProperty = KeyInterop.VirtualKeyFromKey(key);
                    }
                }
            }
        }
        private void KeyMapping_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.OriginalSource is Canvas)
            {
                foreach (UIElement element in canvasMain.Children)
                {
                    if (element is UserButton)
                    {
                        UserButton btn          = element as UserButton;
                        if (btn.valueProperty == KeyInterop.VirtualKeyFromKey(Key.None))
                        {
                            newUserBtnTemp = btn;
                            newUserBtnTemp.textbox.Focus();
                        }
                    }
                }

                if (newUserBtnTemp != null)
                {   //如果当前有处于编辑状态按钮
                    if (newUserBtnTemp.valueProperty != KeyInterop.VirtualKeyFromKey(Key.None))
                    {  //如果已经配置按键

                        createUserButton(true);
                    }
                }
                else if (e.LeftButton == MouseButtonState.Pressed)
                {   //如果当前没有处于编辑状态按钮
                    createUserButton(true);
                }

                if (newUserBtnTemp != null)
                {
                    newUserBtnTemp.SetValue(Canvas.LeftProperty, e.GetPosition(canvasMain).X - 24);
                    newUserBtnTemp.SetValue(Canvas.TopProperty, e.GetPosition(canvasMain).Y - 24);
                }
            }
        }
        private void KeyMapping_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Point topleft       = PointToScreen(new Point(0, 0));
            Point botright      = PointToScreen(new Point(e.NewSize.Width, e.NewSize.Height));
            _radioX             = 1280/(botright.X - topleft.X);
            _radioY             = 720/(botright.Y - topleft.Y);

            imgAim.SetValue(Canvas.LeftProperty, e.NewSize.Width - 8 * 50);
            imgFire.SetValue(Canvas.LeftProperty, e.NewSize.Width - 7 * 50);
            imgDirection.SetValue(Canvas.LeftProperty, e.NewSize.Width - 6 * 50);
            btnClear.SetValue(Canvas.LeftProperty, e.NewSize.Width - 5 * 50);
            btnSave.SetValue(Canvas.LeftProperty, e.NewSize.Width - 3 * 50);
            btnClose.SetValue(Canvas.LeftProperty, e.NewSize.Width - 1 * 50);
        }        
        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Visibility = Visibility.Hidden;
        }
        private void UncheckedUserBtn()
        {
            foreach (UIElement element in canvasMain.Children)
            {
                if (element is UserButton)
                {
                    UserButton btn          = element as UserButton;
                    //btn.btn.IsChecked       = false;
                }
            }
        }
        private void menuDel_Click(object sender, RoutedEventArgs e)
        {
            var mi                          = sender as MenuItem;
            var cm                          = mi.Parent as ContextMenu;
            var textbox                     = cm.PlacementTarget as TextBox;
            var grid                        = textbox.Parent as Grid;
            var userBtn                     = grid.Parent as UserButton;
            newUserBtnTemp                  = null;

            canvasMain.Children.Remove(userBtn);
        }
    }
}
