
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.DirectoryServices;

namespace PrimaGUI
{
    class Transparent : System.Windows.Forms.DataGridView
    {


        protected override void PaintBackground(Graphics graphics, Rectangle clipBounds, Rectangle gridBounds)
        {
            base.PaintBackground(graphics, clipBounds, gridBounds);
            Rectangle rectSource = new Rectangle(this.Location.X, this.Location.Y, this.Width, this.Height);
            Rectangle rectDest = new Rectangle(0, 0, rectSource.Width, rectSource.Height);

            Bitmap b = new Bitmap(Parent.ClientRectangle.Width, Parent.ClientRectangle.Height);
            Graphics.FromImage(b).DrawImage(this.Parent.BackgroundImage, Parent.ClientRectangle);


            graphics.DrawImage(b, rectDest, rectSource, GraphicsUnit.Pixel);
            SetCellsTransparent();
        }


        public void SetCellsTransparent()
        {
            foreach (DataGridViewColumn col in this.Columns)
            {
                col.DefaultCellStyle.BackColor = Color.Transparent;
                col.DefaultCellStyle.SelectionBackColor = Color.Transparent;
            }
        }

        public int PageSize
        {
            get
            {
                return _pageSize;
            }
            set
            {
                _pageSize = value;
                bs = new BindingSource();
                tables = new BindingList<DataTable>(); 
            }
        }
        public int _pageSize = 10;
        BindingSource bs = new BindingSource();
        BindingList<DataTable> tables = new BindingList<DataTable>();
        public void SetPagedDataSource(DataTable dataTable, BindingNavigator bnav)
        {
            DataTable dt = null;
            int counter = 1;
            foreach (DataRow dr in dataTable.Rows)
            {
                if (counter == 1)
                {
                    dt = dataTable.Clone();
                    tables.Add(dt);
                }
                dt.Rows.Add(dr.ItemArray);
                if (PageSize < ++counter)
                {
                    counter = 1;
                }
            }

            bnav.BindingSource = bs;
            bs.DataSource = tables;
            bs.PositionChanged += bs_PositionChanged;
            bs_PositionChanged(bs, EventArgs.Empty);
        }
        void bs_PositionChanged(object sender, EventArgs e)
        {
            this.DataSource = tables[bs.Position];
        }
    }
}