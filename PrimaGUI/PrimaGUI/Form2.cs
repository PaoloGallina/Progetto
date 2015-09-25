using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PrimaGUI
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = true;
            button4.Visible = true;
            label1.Visible = true;
            label2.Visible = true;
            label3.Visible = false;
            textBox1.Visible = true;
            textBox2.Visible = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = true;
            button4.Visible = true;
            button5.Visible = true;
            label1.Visible = true;
            label2.Visible = true;
            label3.Visible = false;
            textBox1.Visible = true;
            textBox2.Visible = true;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            button1.Visible = true;
            button2.Visible = true;
            button3.Visible = false;
            button4.Visible = false;
            button5.Visible = false;
            label1.Visible = false;
            label2.Visible = false;
            label3.Visible = true;
            label4.Visible = false;
            textBox1.Visible = false;
            textBox2.Visible = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "" || textBox2.Text == ""||(Program.path==""&&button5.Visible==true)) {
                if (button5.Visible == false)
                {
                    label4.Text = "INSERISCI TUTTI I CAMPI E CONTROLLA CHE SIANO CORRETTI";
                }
                else {
                    label4.Text = "INSERISCI TUTTI I CAMPI E SELEZIONA LA CARTELLA DA CONTROLLARE";
                }
                label4.Visible = true;
                return;
            }
            // Questo attualmente è solo uno stub! naturalmente oltre a controllare che siano campi vuoti bisogna fare un check della correttezza

            Program.userName = textBox1.Text;
            Program.Password = textBox2.Text;
            this.Close();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            Program.path = folderBrowserDialog1.SelectedPath;
        }


    
    }
}
