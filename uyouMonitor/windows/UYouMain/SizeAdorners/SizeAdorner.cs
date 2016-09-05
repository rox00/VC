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
    class SizeAdorner : Adorner
    {
        private SizeChrome          chrome;
        private VisualCollection    visuals;
        private ContentControl      designerItem;

        protected override int VisualChildrenCount
        {
            get
            {
                return this.visuals.Count;
            }
        }

        public SizeAdorner(ContentControl designerItem)
            : base(designerItem)
        {
            this.SnapsToDevicePixels    = true;
            this.designerItem           = designerItem;
            this.chrome                 = new SizeChrome();
            this.chrome.DataContext     = designerItem;
            this.visuals                = new VisualCollection(this);
            this.visuals.Add(this.chrome);
        }

        protected override System.Windows.Size ArrangeOverride(System.Windows.Size finalSize)
        {
            this.chrome.Arrange(new Rect(finalSize));
            return finalSize;
        }

        protected override Visual GetVisualChild(int index)
        {
            return visuals[index];
        }
    }
}
