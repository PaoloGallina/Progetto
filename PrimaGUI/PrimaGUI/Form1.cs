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
using System.IO;
using System.IO.Pipes;

         

namespace PrimaGUI
{
    public partial class Form1 : Form
    {
        public AutoResetEvent T = new AutoResetEvent(false);
        private NamedPipeServerStream PServer1 = null;
        private StreamReader St=null;
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
            
        while (this.PServer1 ==null||St==null){
                PServer1 = new NamedPipeServerStream("myNamedPipe1", System.IO.Pipes.PipeDirection.InOut);
                PServer1.WaitForConnection();
                this.St = new StreamReader(PServer1,System.Text.Encoding.Unicode);
            }

            while (true) {
                string nuovo_path=St.ReadLine();
                if (nuovo_path.CompareTo(@"stop") == 0)
                {
                    break;
                }
                Label a = new Label();
                a.AutoSize = true;
                a.Text = nuovo_path;
                flowLayoutPanel1.Controls.Add(a);
                
            }

        }


    }
}
