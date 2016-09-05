using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;

namespace UYouMain.Adorners
{
    class DesignerItemDecorator : Control
    {
        private SizeAdorner adorner;

        public bool ShowDecorator
        {
            get { return (bool)GetValue(ShowDecoratorProperty); }
            set { SetValue(ShowDecoratorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ShowDecorator.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ShowDecoratorProperty = 
    DependencyProperty.Register("ShowDecorator", typeof(bool), typeof(DesignerItemDecorator), new FrameworkPropertyMetadata(false, new PropertyChangedCallback(ShowDecoratorProperty_Changed)));


        private static void ShowDecoratorProperty_Changed(DependencyObject obj, DependencyPropertyChangedEventArgs args)
        {
            DesignerItemDecorator decorator = (DesignerItemDecorator)obj;

            bool showDecorator = (bool)args.NewValue;
            if(showDecorator)
            {
                decorator.ShowAdorner();
            }
            else
            {
                decorator.HideAdorner();
            }
        }
        
        private void HideAdorner()
        {
            if(this.adorner != null)
            {
                this.adorner.Visibility = Visibility.Hidden;
            }
        }

        private void ShowAdorner()
        {
            if(this.adorner == null)
            {
                AdornerLayer adornerLayer = AdornerLayer.GetAdornerLayer(this);

                if(adornerLayer != null)
                {
                    ContentControl designerItem = this.DataContext as ContentControl;
                    Canvas canvas               = VisualTreeHelper.GetParent(designerItem) as Canvas;
                    this.adorner                = new SizeAdorner(designerItem);
                    adornerLayer.Add(this.adorner);

                    if(this.ShowDecorator)
                    {
                        this.adorner.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        this.adorner.Visibility = Visibility.Hidden;
                    }
                }
            }
            else
            {
                this.adorner.Visibility = Visibility.Visible;
            }
        }

    }
}
