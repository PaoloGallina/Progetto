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
        private bool flag = false;
        private bool _dragging = false;
        private Point _start_point = new Point(0, 0);

        public Form2()
        {
            
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.None;
        }

        private void Login_Click(object sender, EventArgs e)
        {
            LoginButton.Visible = false;
            RegisterButton.Visible = false;
            OkButton.Visible = true;
            IndietroButton.Visible = true;
            HelpButtonN.Visible = false;
            AboutButton.Visible = false;
            label3.Visible = false;
            label4.Visible = true;
            UsernameText.Visible = true;
            PasswordText.Visible = true;
            IpAddressText.Visible = true;
           

        }

        private void register_Click(object sender, EventArgs e)
        {
            LoginButton.Visible = false;
            RegisterButton.Visible = false;
            OkButton.Visible = true;
            IndietroButton.Visible = true;
            HelpButtonN.Visible = false;
            AboutButton.Visible = false;
            CartDaSyncButton.Visible = true;
            label3.Visible = false;
            label4.Visible = true;
            UsernameText.Visible = true;
            PasswordText.Visible = true;
            IpAddressText.Visible = true;
     

        }

        private void Indietro_Click(object sender, EventArgs e)
        {
            LoginButton.Visible = true;
            RegisterButton.Visible = true;
            OkButton.Visible = false;
            IndietroButton.Visible = false;
            CartDaSyncButton.Visible = false;
            HelpButtonN.Visible = true;
            AboutButton.Visible = true;
            label3.Visible = true;
            label4.Visible = false;
            infoLabel.Visible = false;
            UsernameText.Visible = false;
            PasswordText.Visible = false;
            IpAddressText.Visible = false;
        }

        private void OK_Click(object sender, EventArgs e)
        {
            if (UsernameText.Text == "Username" ||!IsValidalfa(UsernameText.Text)||!IsValidalfa(PasswordText.Text)|| ! IsValidIP(IpAddressText.Text) || PasswordText.Text == "Password" || (Program.path == "" && CartDaSyncButton.Visible == true))
            { 
                label4.Text = "Inserisci tutti i campi.";
                label4.ForeColor = System.Drawing.Color.Red;
                return;     
            } 
            
            int logORreg = 40;
            if (CartDaSyncButton.Visible == true) {
                logORreg = 30;
            }

            Program.Bin.Write(IpAddressText.Text.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(IpAddressText.Text));
            string temp = Program.Sr.ReadLine();
            if (temp.CompareTo("OK") != 0)
            {
                label4.Text = temp;
                label4.ForeColor = System.Drawing.Color.Red;
                return;
            }
            Program.Bin.Write(logORreg);
            Program.Bin.Write(UsernameText.Text.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(UsernameText.Text));
            Program.Bin.Write(PasswordText.Text.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(PasswordText.Text));
            
            
            temp=Program.Sr.ReadLine();
            if (temp.CompareTo("OK")!=0) {
                label4.Text = temp;
                label4.ForeColor = System.Drawing.Color.Red;
                return;
            }

            Program.userName = UsernameText.Text;
            Program.Password = PasswordText.Text;
            Program.path     = folderBrowserDialog1.SelectedPath;
            Program.ip = IpAddressText.Text;

            if (CartDaSyncButton.Visible == true){ // i.e. Ho effettuato la registrazione
                try
                {
                    using (System.IO.StreamWriter file = new System.IO.StreamWriter(@".\_" + UsernameText.Text + @"_Config_.bin", false, System.Text.Encoding.Unicode))
                    {
                        file.WriteLine(Program.path);
                     }
                }
                catch {
                    label4.Text = "FILE CONFIG ERROR";
                }
            }

            flag = true;
            this.Close();
            
        }

        private void ChooseFolder_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            Program.path = folderBrowserDialog1.SelectedPath;
        }

        private void About_Click(object sender, EventArgs e)
        {
            LoginButton.Visible = false;
            RegisterButton.Visible = false;
            OkButton.Visible = false;
            IndietroButton.Visible = true;
            CartDaSyncButton.Visible = false;
            HelpButtonN.Visible = false;
            AboutButton.Visible = false;
            label3.Visible = false;
            label4.Visible = false;
            UsernameText.Visible = false;
            PasswordText.Visible = false;
            IpAddressText.Visible = false;
            infoLabel.Visible = true;
            infoLabel.Text = "Il core del programma è scritto in c++ la GUI in c#, sono state utilizzate librerie open source di terze parti per calcolare l'hash e per gestire il database interno.\nL'intento è creare un servizio di backup e versioning automatico di una cartella che permetta di recuperare versioni passate o file cancellati.";
        }

        private void Help_Click(object sender, EventArgs e)
        {

            LoginButton.Visible = false;
            RegisterButton.Visible = false;
            OkButton.Visible = false;
            IndietroButton.Visible = true;
            CartDaSyncButton.Visible = false;
            HelpButtonN.Visible = false;
            AboutButton.Visible = false;
            label3.Visible = false;
            label4.Visible = false;
            UsernameText.Visible = false;
            PasswordText.Visible = false;
            IpAddressText.Visible = false;
            infoLabel.Visible = true;
            infoLabel.Text = "Stai visualizzando la procedura di login del programma.\n\nSe non lo hai già fatto puoi procedere alla registrazione attraverso il corrispondente pannello scegliendo uno username, una password e la cartellla da sincronizzare.\nAl contrario puoi procedere al login.\n";
        }
        
        private void UserText_TextChanged(object sender, EventArgs e)
        {
            UsernameText.ForeColor = SystemColors.WindowText;
        }

        private void PassText_TextChanged(object sender, EventArgs e)
        {
            PasswordText.ForeColor = SystemColors.WindowText;
            PasswordText.UseSystemPasswordChar = true;
        }

        private void Iptext_TextChanged(object sender, EventArgs e)
        {
            IpAddressText.ForeColor = SystemColors.WindowText;
        }

        private void UserText_MouseClick(object sender, MouseEventArgs e)
        {
            UsernameText.SelectAll();
        }

        private void PassText_MouseClick(object sender, MouseEventArgs e)
        {
            PasswordText.SelectAll();
        }

        private void Iptext_MouseClick(object sender, MouseEventArgs e)
        {
            IpAddressText.SelectAll();
        }

        private void ControlloEnter_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar==(char)13) {
                OK_Click(sender, e);
            }
        }

        //Necessario per la chiusura del processo client C++
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            if (flag == false)
            {
                Program.myprocess.Kill();
            }
            base.OnClosing(e);
        }

        //I tre metodi per rendere spostabile il pannello
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

        //I due metodi per controllare i pattern
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


    }
}
