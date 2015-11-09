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
    public partial class Form1 : Form
    {
        public AutoResetEvent T = new AutoResetEvent(false);

        public Form1()
        {
            InitializeComponent();
            this.Text = "Benvenuto " + Program.userName+" controllando "+Program.path;
            this.FormBorderStyle = FormBorderStyle.None;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Visible == false)
            {
                dataGridView1.Visible = true;
                dataGridView2.Visible = true;
            }
            else
            {
                dataGridView1.Visible = false;
                dataGridView2.Visible = false; 
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            Program.path = folderBrowserDialog1.SelectedPath;
        }



    }
}
