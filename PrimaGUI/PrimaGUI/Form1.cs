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

        private void label1_Click(object sender, EventArgs e)
        {
        }

        private void button1_KeyDown(object sender, KeyEventArgs e)
        {
        }

        private void sviluppatoreToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            pictureBox1.Visible = true;
            pictureBox2.Visible = true;
            label1.Visible = true;
            label1.Text = "Sono uno studente del politcnico di Torino\nAttualmente frequento il secondo anno della laurea in ingegneria informatica\n"
            +"Questo progetto è sviluppato per conto di un progetto all'interti del corso Programmazione di sistema";
            
        }

        private void pictureBox1_Click(object sender, EventArgs e)
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


    }
}
