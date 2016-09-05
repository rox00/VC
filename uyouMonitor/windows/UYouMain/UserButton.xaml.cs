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

namespace UYouMain
{
    /// <summary>
    /// UserButton.xaml 的交互逻辑
    /// </summary>
    public partial class UserButton : UserControl
    {
        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register("DirectionKeyProperty", typeof(int), typeof(UserButton),
            new FrameworkPropertyMetadata(0, new PropertyChangedCallback(ValuePropertyChangedCallback)));

        public static readonly RoutedEvent ValuePropertyChangedEvent =
            EventManager.RegisterRoutedEvent("ValuePropertyChanged",
             RoutingStrategy.Bubble, typeof(RoutedPropertyChangedEventHandler<int>), typeof(UserButton));

        public int valueProperty
        {
            get { return (int)this.GetValue(ValueProperty); }
            set { this.SetValue(ValueProperty, value); }
        }

        public event RoutedPropertyChangedEventHandler<int> ValuePropertyChanged
        {
            add
            {
                this.AddHandler(ValuePropertyChangedEvent, value);
            }
            remove
            {
                this.RemoveHandler(ValuePropertyChangedEvent, value);
            }
        }

        private static void ValuePropertyChangedCallback(DependencyObject sender, DependencyPropertyChangedEventArgs arg)
        {
            if (sender != null && sender is UserButton)
            {
                UserButton clock = sender as UserButton;
                clock.OnValuePropertyChanged((int)arg.OldValue, (int)arg.NewValue);                
            }
        }

        protected virtual void OnValuePropertyChanged(int oldValue, int newValue)
        {
            RoutedPropertyChangedEventArgs<int> arg =
                new RoutedPropertyChangedEventArgs<int>(oldValue, newValue, ValuePropertyChangedEvent);
            this.RaiseEvent(arg);

            Key key = KeyInterop.KeyFromVirtualKey(newValue);

            if (key == Key.None)
            {
                valueProperty   = KeyInterop.VirtualKeyFromKey(Key.None);
                textbox.Text    = "";
            }
            else
            {
                textbox.Text = key.ToString();
                textbox.SelectionStart = textbox.Text.Length;
            }
        }
        
        public UserButton()
        {
            InitializeComponent();
            this.valueProperty      = KeyInterop.VirtualKeyFromKey(Key.None);
        }

        public UserButton(bool isChecked){
            InitializeComponent();
            this.valueProperty      = KeyInterop.VirtualKeyFromKey(Key.None);
            //btn.IsChecked           = isChecked;

            this.Loaded             += UserButton_Loaded;
            textbox.PreviewKeyDown  += textbox_PreviewKeyDown;
        }

        void UserButton_Loaded(object sender, RoutedEventArgs e)
        {
            textbox.Focus();
        }

        void textbox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
        }
    }
}
