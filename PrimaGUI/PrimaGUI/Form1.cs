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
        }

        private void sviluppatoreToolStripMenuItem_Click(object sender, EventArgs e)
        {
              
        }


        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void chiudiToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void process1_Exited(object sender, EventArgs e)
        {

        }

        private void internetToolStripMenuItem_Click(object sender, EventArgs e)
        {
     
        }

        private void scegliCartellaDaControllareToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }



        private void flowLayoutPanel1_DoubleClick(object sender, EventArgs e)
        {
        
        }


    }
}
