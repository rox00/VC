using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace UYouMain.Adorners
{
    public class DoubleFormatConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            double d = (double)value;
            return Math.Round(d);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return null;
        }
    }

    class SizeChrome : Control
    {
        public double ThumbLineWidth
        {
            get { return (double)GetValue(ThumbLineWidthProperty); }
            set { SetValue(ThumbLineWidthProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ThumbLineWidth.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ThumbLineWidthProperty =
            DependencyProperty.Register("ThumbLineWidth", typeof(double), typeof(SizeChrome), new PropertyMetadata(10.0));


        public double ThumbLineHeight
        {
            get { return (double)GetValue(ThumbLineHeightProperty); }
            set { SetValue(ThumbLineHeightProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ThumbLineHeight.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ThumbLineHeightProperty =
            DependencyProperty.Register("ThumbLineHeight", typeof(double), typeof(SizeChrome), new PropertyMetadata(10.0));


        public SizeChrome()
            : base()
        {
            this.SizeChanged += SizeChrome_SizeChanged;
        }

        void SizeChrome_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            ThumbLineWidth  = (e.NewSize.Width - 16) / 2;
            ThumbLineHeight = (e.NewSize.Height - 16) / 2;
        }
    }
}
