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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Controls.Primitives;
using System.Globalization;
using System.ComponentModel;

namespace UYouMain
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

    /// <summary>
    /// DirectionButton.xaml 的交互逻辑
    /// </summary>
    public partial class DirectionButton : UserControl
    {
        
        private static Dictionary<String, String> directionValue = new Dictionary<string, string>();

        public static readonly DependencyProperty CurrentDirection =
            DependencyProperty.Register("CurrentDirection", typeof(String), typeof(DirectionButton),
            new FrameworkPropertyMetadata(null, new PropertyChangedCallback(CurrentDirectionPropertyChangedCallback)));

        private static void CurrentDirectionPropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (sender != null && sender is DirectionButton)
            {
                DirectionButton clock = sender as DirectionButton;
                clock.OnCurrentDirectionChanged((String)arg.OldValue, (String)arg.NewValue);
            }
        }
        protected virtual void OnCurrentDirectionChanged(String oldValue, String newValue)
        {
            RoutedPropertyChangedEventArgs<String> arg =
                new RoutedPropertyChangedEventArgs<String>(oldValue, newValue, CurrentDirectionEvent);
            this.RaiseEvent(arg);

        }

        public static readonly RoutedEvent CurrentDirectionEvent =
            EventManager.RegisterRoutedEvent("CurrentDirectionChanged",
             RoutingStrategy.Bubble, typeof(RoutedPropertyChangedEventHandler<String>), typeof(DirectionButton));

        
        public String currentDirection
        {
            get
            {
                return (String)this.GetValue(CurrentDirection);
            }
            set
            {
                this.SetValue(CurrentDirection, value);
            }
        }
        public event RoutedPropertyChangedEventHandler<String> CurrentDirectionChanged
        {
            add
            {
                this.AddHandler(CurrentDirectionEvent, value);
            }
            remove
            {
                this.RemoveHandler(CurrentDirectionEvent, value);
            }
        }

        
        public class DirectionKey 
        {
            private int up          = KeyInterop.VirtualKeyFromKey(Key.W);
            private int left        = KeyInterop.VirtualKeyFromKey(Key.A);
            private int right       = KeyInterop.VirtualKeyFromKey(Key.S);
            private int down        = KeyInterop.VirtualKeyFromKey(Key.D);
            
            private Point upPos     = new Point(0, 0);
            private Point leftPos   = new Point(0, 0);
            private Point rightPos  = new Point(0, 0);
            private Point downPos   = new Point(0, 0);

            private Point vbtnSize  = new Point(0, 0);
            private Point hbtnSize  = new Point( 0, 0 );
            public Point UpPos
            {
                get { return upPos; }
                set { upPos = value; }
            }

            public Point LeftPos
            {
                get { return leftPos; }
                set { leftPos = value; }
            }

            public Point RightPos
            {
                get { return rightPos; }
                set { rightPos = value; }
            }

            public Point DownPos
            {
                get { return downPos; }
                set { downPos = value; }
            }

            public Point VBtnSize
            {
                get { return vbtnSize; }
                set { vbtnSize = value; }
            }

            public Point HBtnSize
            {
                get { return hbtnSize; }
                set { hbtnSize = value; }
            }

            public int Up{
                get{ return up; }
                set{
                    if (up != value)
                    {
                        up = value;            
                    }
                }
            }

            public int Left
            {
                get{ return left; }
                set{
                    if (left != value)
                    {
                        left = value;                        
                    }
                }
            }

            public int Right
            {
                get { return right; }
                set { 
                    if (right != value){
                        right = value;                        
                    }
                }
            }

            public int Down
            {
                get { return down; }
                set {
                    if (down != value)
                    {
                        down = value;                        
                    }
                }
            }

        }

        public static readonly DependencyProperty DirectionKeyProperty =
            DependencyProperty.Register("DirectionKeyProperty", typeof(DirectionKey), typeof(DirectionButton),
            new FrameworkPropertyMetadata(new DirectionKey(), new PropertyChangedCallback(DirectionKeyPropertyChangedCallback)));

        public static readonly RoutedEvent DirectionKeyPropertyChangedEvent =
            EventManager.RegisterRoutedEvent("DirectionKeyPropertyChanged",
             RoutingStrategy.Bubble, typeof(RoutedPropertyChangedEventHandler<DirectionKey>), typeof(DirectionButton));

        public DirectionKey directionKeyProperty { 
            get { return (DirectionKey)this.GetValue(DirectionKeyProperty); }
            set { this.SetValue(DirectionKeyProperty, value); }
        }
                
        public event RoutedPropertyChangedEventHandler<Key> DirectionKeyPropertyChanged
        {
            add
            {
                this.AddHandler(DirectionKeyPropertyChangedEvent, value);
            }
            remove
            {
                this.RemoveHandler(DirectionKeyPropertyChangedEvent, value);
            }
        }
        
        private static void DirectionKeyPropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (sender != null && sender is DirectionButton)
            {
                DirectionButton clock = sender as DirectionButton;
                clock.OnDirectionKeyPropertyChanged((DirectionKey)arg.OldValue, (DirectionKey)arg.NewValue);

            }
        }

        protected virtual void OnDirectionKeyPropertyChanged(DirectionKey oldValue, DirectionKey newValue)
        {
            RoutedPropertyChangedEventArgs<DirectionKey> arg =
                new RoutedPropertyChangedEventArgs<DirectionKey>(oldValue, newValue, DirectionKeyPropertyChangedEvent);
            this.RaiseEvent(arg);
            grid1.DataContext = newValue;            
        }

        
        public DirectionButton()
        {
            InitializeComponent();
            /*
            btnUp.Click += directBtn_Click;
            btnLeft.Click += directBtn_Click;
            btnRight.Click += directBtn_Click;
            btnDown.Click += directBtn_Click;            
            */
            directionKeyProperty.Up = KeyInterop.VirtualKeyFromKey(Key.W);
            directionKeyProperty.Left = KeyInterop.VirtualKeyFromKey(Key.A);
            directionKeyProperty.Right = KeyInterop.VirtualKeyFromKey(Key.D);
            directionKeyProperty.Down = KeyInterop.VirtualKeyFromKey(Key.S);            
        }

        void directionKeyProperty_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            grid1.DataContext = directionKeyProperty;
        }

        private void cancelAllCheckedStatus()
        {
            /*
            ToggleButton[] btns = new ToggleButton[] { btnLeft, btnUp, btnRight, btnDown };
            foreach (ToggleButton item in btns)
            {                
                item.IsChecked = false;
            }
            currentDirection = null;
            */
        }

        private void changeCheckedStatus(ToggleButton srcBtn, Boolean isChecked){
            /*
            ToggleButton[] btns = new ToggleButton[] { btnLeft, btnUp, btnRight, btnDown };
            srcBtn.IsChecked = isChecked;
            if (isChecked)
            {
                foreach(ToggleButton item in btns){
                    if (item == srcBtn){
                        currentDirection = srcBtn.Name;
                        continue;
                    }
                    item.IsChecked = false;
                }    
            }
            else
            {
                currentDirection = null;
            }
            */
        }

        void directBtn_Click(object sender, RoutedEventArgs e)
        {            
            /*
            ToggleButton btn = sender as ToggleButton;
            bool isChecked = btn.IsChecked.Value;
            changeCheckedStatus(sender as ToggleButton, isChecked);
            */
        }              

        private void UserControl_KeyDown(object sender, KeyEventArgs e)
        {
            return;
            /*
            if (currentDirection != null)
            {
                DirectionKey newValue = new DirectionKey();
                newValue.Left = directionKeyProperty.Left;
                newValue.Up = directionKeyProperty.Up;
                newValue.Down = directionKeyProperty.Down;
                newValue.Right = directionKeyProperty.Right;
                
                if (currentDirection == btnUp.Name)
                {                    
                    
                    if (newValue.Down == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Left == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Right == KeyInterop.VirtualKeyFromKey(e.Key))
                    {
                        
                    }
                    else
                    {
                        newValue.Up = KeyInterop.VirtualKeyFromKey(e.Key);
                    }                    
                }

                else if (currentDirection == btnLeft.Name)
                {

                    if (newValue.Down == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Up == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Right == KeyInterop.VirtualKeyFromKey(e.Key))
                    {

                    }
                    else
                    {
                        newValue.Left = KeyInterop.VirtualKeyFromKey(e.Key);
                    }
                                        
                }
                else if(currentDirection == btnRight.Name){
                    
                    if (newValue.Down == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Left == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Up == KeyInterop.VirtualKeyFromKey(e.Key))
                    {                        
                    }
                    else{
                        newValue.Right = KeyInterop.VirtualKeyFromKey(e.Key);
                    }                    
                }
                else if(currentDirection == btnDown.Name){
                    
                    if (newValue.Up == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Left == KeyInterop.VirtualKeyFromKey(e.Key)
                        || newValue.Right == KeyInterop.VirtualKeyFromKey(e.Key)
                    )
                    {
                        
                    }
                    else{
                        newValue.Down = KeyInterop.VirtualKeyFromKey(e.Key);
                    }                                        
                }
                directionKeyProperty = newValue;
            }
            */
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            grid1.DataContext = directionKeyProperty;
            double width = grid1.ActualWidth / 3;
            double height = grid1.ActualHeight / 3;

            Point pos = new Point(0, height);
            directionKeyProperty.LeftPos = pos;
            pos = new Point(width, 0);
            directionKeyProperty.UpPos = pos;
            pos = new Point(2 * width, height);
            directionKeyProperty.RightPos = pos;
            pos = new Point(width, 2 * height);
            directionKeyProperty.DownPos = pos;

            pos = new Point(width, height);
            directionKeyProperty.VBtnSize = pos;

            pos = new Point(height, width);
            directionKeyProperty.HBtnSize = pos;
        }
    }
}
