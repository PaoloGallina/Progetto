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
        private bool _dragging = false;
        private Point _offset;
        private Point _start_point = new Point(0, 0);

        public Form2()
        {
            
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.None;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.BackgroundImage = global::PrimaGUI.Properties.Resources._9ToJXnJ;
            Login.Visible = false;
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
            Login.Visible = false;
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
            Login.Visible = true;
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
            
            string temp = "login";
            if (button5.Visible == true) {
                 temp = "register";
            }

            Program.Bin.Write(temp.Length);
            Program.Bin.Write(temp);
            Program.Bin.Write(textBox1.Text.Length);
            Program.Bin.Write(textBox1.Text);
            Program.Bin.Write(textBox2.Text.Length);
            Program.Bin.Write(textBox2.Text);
            
            temp=Program.Sr.ReadLine();
            if (temp.CompareTo("errore")==0) {
                label4.Text = "errore";
                label4.ForeColor = System.Drawing.Color.Red;
                return;
            }

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
        //  Form1 f=new Form1();
        //  f.ShowDialog();
        //  f.Close();
            this.BackgroundImage = global::PrimaGUI.Properties.Resources._9ToJXnJ;
            Login.Visible = false;
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
            Login.Visible = false;
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

        private void textBox1_MouseClick(object sender, MouseEventArgs e)
        {

                textBox1.Text = "";
                textBox1.ForeColor = SystemColors.WindowText;
        }
        

        private void textBox2_MouseClick(object sender, MouseEventArgs e)
        {

                textBox2.Text = "";
                textBox2.UseSystemPasswordChar = true;
                textBox2.ForeColor = SystemColors.WindowText;
        }

        private void portatext_MouseClick(object sender, MouseEventArgs e)
        {
                portatext.Text = "";
                portatext.ForeColor = SystemColors.WindowText;
        }

        private void iptext_MouseClick(object sender, MouseEventArgs e)
        {
                iptext.Text = "";
                iptext.ForeColor = SystemColors.WindowText;
        }


        private void panel2_MouseDown(object sender, MouseEventArgs e)
        {
            _dragging = true;  // _dragging is your variable flag
            _start_point = new Point(e.X, e.Y);
        }

        private void panel2_MouseUp(object sender, MouseEventArgs e)
        {
            _dragging = false;
        }

        private void panel2_MouseMove(object sender, MouseEventArgs e)
        {
            if (_dragging)
            {
                Point p = PointToScreen(e.Location);
                Location = new Point(p.X - this._start_point.X, p.Y - this._start_point.Y);
            }
        }

        private void textBox1_Enter(object sender, EventArgs e)
        {
            textBox1.ForeColor = SystemColors.WindowText;
            
        }

        private void textBox2_Enter(object sender, EventArgs e)
        {
            textBox2.UseSystemPasswordChar = true;
            textBox2.ForeColor = SystemColors.WindowText;

        }

        private void portatext_Enter(object sender, EventArgs e)
        {
            portatext.ForeColor = SystemColors.WindowText;

        }

        private void iptext_Enter(object sender, EventArgs e)
        {  
            iptext.ForeColor = SystemColors.WindowText;
        }

    }
}
