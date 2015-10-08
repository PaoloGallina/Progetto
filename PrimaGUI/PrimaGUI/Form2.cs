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
            this.BackgroundImage = global::PrimaGUI.Properties.Resources._9ToJXnJ;
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = true;
            button4.Visible = true;
            Help.Visible = false;
            About.Visible = false;
            label3.Visible = false;
            label4.Visible = true;
            textBox1.Visible = true;
            textBox2.Visible = true;
            iptext.Visible = true;

            portatext.Visible = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {

            this.BackgroundImage = global::PrimaGUI.Properties.Resources.rZWcYhh; 
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = true;
            button4.Visible = true;
            Help.Visible = false;
            About.Visible = false;
            button5.Visible = true;
            label3.Visible = false;
            label4.Visible = true;
            textBox1.Visible = true;
            textBox2.Visible = true;
            iptext.Visible = true;
            portatext.Visible = true;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            this.BackgroundImage = global::PrimaGUI.Properties.Resources.x5j8KEF;
            button1.Visible = true;
            button2.Visible = true;
            button3.Visible = false;
            button4.Visible = false;
            button5.Visible = false;
            Help.Visible = true;
            About.Visible = true;
            label3.Visible = true;
            label4.Visible = false;
            infoLabel.Visible = false;
            textBox1.Visible = false;
            textBox2.Visible = false;
            iptext.Visible = false;
            portatext.Visible = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "Username" || portatext.Text=="Port Number" || portatext.Text=="Ip address" || textBox2.Text == "Password"||(Program.path==""&&button5.Visible==true)) {
                if (button5.Visible == false)
                {
                    label4.Text = "Inserisci tutti i campi";
                }
                else {
                    label4.Text = "Inserisci tutti i campi";
                }
                label4.ForeColor = System.Drawing.Color.Red;
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

        private void About_Click(object sender, EventArgs e)
        {
            Form1 f=new Form1();
            f.ShowDialog();
            this.BackgroundImage = global::PrimaGUI.Properties.Resources._9ToJXnJ;
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = false;
            button4.Visible = true;
            button5.Visible = false;
            Help.Visible = false;
            About.Visible = false;
            label3.Visible = false;
            label4.Visible = false;
            textBox1.Visible = false;
            textBox2.Visible = false;
            iptext.Visible = false;
            portatext.Visible = false;
            infoLabel.Visible = true;
            infoLabel.Text = "Questo programma è stato sviluppato interamente da Paolo Gallina come progetto del corso di Programmazione di Sistema\n\nIl core del programma è scritto in c++ la gui in c#, sono state utilizzate librerie open source di terze parti per calcolare l'hash e per gestire il database interno.\n\nL'intento è creare un servizio di backup e versioning automatico di una cartella che permetta di recuperare versioni passate o file cancellati.";
        }

        private void Help_Click(object sender, EventArgs e)
        {

            this.BackgroundImage = global::PrimaGUI.Properties.Resources._9ToJXnJ;
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = false;
            button4.Visible = true;
            button5.Visible = false;
            Help.Visible = false;
            About.Visible = false;
            label3.Visible = false;
            label4.Visible = false;
            textBox1.Visible = false;
            textBox2.Visible = false;
            iptext.Visible = false;
            portatext.Visible = false;
            infoLabel.Visible = true;
            infoLabel.Text = "Stai visualizzando la procedura di login del programma.\n\nSe non lo hai già fatto puoi procedere alla registrazione attraverso il corrispondente pannello scegliendo uno username, una password e la cartellla da sincronizzare.\n\nAl contrario puoi procedere al login.\n\nSe hai dimenticato le credenziali contatta lo Sviluppatore.";
        }

        private void textBox1_TextChanged(object sender, MouseEventArgs e)
        {
            if (textBox1.Text == "Username")
            {
                textBox1.Text = "";
                textBox1.ForeColor = SystemColors.WindowText;
            }
        }

        private void textBox2_MouseClick(object sender, MouseEventArgs e)
        {
            if (textBox2.Text == "Password")
            {
                textBox2.Text = "";
                textBox2.UseSystemPasswordChar = true;
                textBox2.ForeColor = SystemColors.WindowText;
            }
        }

        private void portatext_MouseClick(object sender, MouseEventArgs e)
        {
            if (portatext.Text == "Port Number")
            {
                portatext.Text = "";
                portatext.ForeColor = SystemColors.WindowText;
            }
        }

        private void iptext_MouseClick(object sender, MouseEventArgs e)
        {
            if (iptext.Text == "Ip address")
            {
                iptext.Text = "";
                iptext.ForeColor = SystemColors.WindowText;
            }
        }

        private void iptext_TextChanged(object sender, EventArgs e)
        {

        }



    
    }
}
