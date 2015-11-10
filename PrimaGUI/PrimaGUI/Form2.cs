using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PrimaGUI
{
    public partial class Form2 : Form
    {
        private bool _dragging = false;
        private Point _start_point = new Point(0, 0);

        public Form2()
        {
            
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.None;
        }

        private void button1_Click(object sender, EventArgs e)
        {
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
           

        }

        private void button2_Click(object sender, EventArgs e)
        {
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
     

        }

        private void button4_Click(object sender, EventArgs e)
        {
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
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "Username" ||!IsValidalfa(textBox1.Text)||!IsValidalfa(textBox2.Text)|| ! IsValidIP(iptext.Text) || textBox2.Text == "Password" || (Program.path == "" && button5.Visible == true))
            { 
                label4.Text = "Inserisci tutti i campi. ASCII ONLY";
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
            infoLabel.Visible = true;
            infoLabel.Text = "Il core del programma è scritto in c++ la GUI in c#, sono state utilizzate librerie open source di terze parti per calcolare l'hash e per gestire il database interno.\nL'intento è creare un servizio di backup e versioning automatico di una cartella che permetta di recuperare versioni passate o file cancellati.";
        }

        private void Help_Click(object sender, EventArgs e)
        {

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
            infoLabel.Visible = true;
            infoLabel.Text = "Stai visualizzando la procedura di login del programma.\n\nSe non lo hai già fatto puoi procedere alla registrazione attraverso il corrispondente pannello scegliendo uno username, una password e la cartellla da sincronizzare.\nAl contrario puoi procedere al login.\n";
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


        public bool IsValidIP(string addr)
        {
            
            string pattern = @"^([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])){3}$";
            Regex check = new Regex(pattern);
            bool valid = false;
            if (addr == "")
            {
                valid = false;
            }
            else
            {

                valid = check.IsMatch(addr, 0);
            }
            return valid;
        }

        public bool IsValidalfa(string addr)
        {

            string pattern = @"[a-zA-Z0-9]+$";
            Regex check = new Regex(pattern);
            bool valid = false;
            if (addr == "")
            {
                valid = false;
            }
            else
            {

                valid = check.IsMatch(addr, 0);
            }
            return valid;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            textBox1.ForeColor = SystemColors.WindowText;
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            textBox2.ForeColor = SystemColors.WindowText;
            textBox2.UseSystemPasswordChar = true;
        }

        private void iptext_TextChanged(object sender, EventArgs e)
        {
            iptext.ForeColor = SystemColors.WindowText;
        }

        private void textBox1_MouseClick(object sender, MouseEventArgs e)
        {
            textBox1.SelectAll();
        }

        private void textBox2_MouseClick(object sender, MouseEventArgs e)
        {
            textBox2.SelectAll();
        }

        private void iptext_MouseClick(object sender, MouseEventArgs e)
        {
            iptext.SelectAll();
        }


    }
}
