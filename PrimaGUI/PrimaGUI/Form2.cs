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

        private BackgroundWorker bw = new BackgroundWorker();
        public Form2()
        {      
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.None;
            bw.WorkerReportsProgress = false;
            bw.WorkerSupportsCancellation = false;
            bw.DoWork += new DoWorkEventHandler(DoWork);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(DoWork_end);
        }

        private void DoWork(object sender, DoWorkEventArgs e) {
            try
            {
                if (!IsValidpass(PasswordText.Text) || PasswordText.Text == "Password")
                {
                    e.Result = "Non hai inserito la password o hai immesso caratteri non validi. Sono ammessi solo caratteri ASCII alfanumerici con aggiunta dei seguenti caratteri speciali !£$%&/()=?^\\ per la password.";
                    return;
                }
                else if (Program.path == "" && CartDaSyncButton.Visible == true)
                {
                    e.Result = "Non hai scelto la cartella da sincronizzare.";
                    return;
                }
                else if (UsernameText.Text == "Username" || !IsValidalfa(UsernameText.Text))
                {
                    e.Result = "Non hai inserito lo username o hai immesso caratteri non validi. Sono ammessi solo caratteri ASCII alfanumerici per lo username.";
                    return;
                }
                else if (!IsValidIP(IpAddressText.Text))
                {
                    e.Result = "L'indirizzo IP inserito non è un indirizzo IP valido.";
                    return;
                }
                else if (!IsValidPort(PortaText.Text))
                {
                    e.Result = "Il numero di porta non è valido.";
                    return;
                }


                int logORreg = 40;
                if (CartDaSyncButton.Visible == true)
                {
                    logORreg = 30;
                }

                Program.Bin.Write(IpAddressText.Text.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(IpAddressText.Text));
                Program.Bin.Write(PortaText.Text.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(PortaText.Text));
               
                
                
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                    return;
                }
                Program.Bin.Write(logORreg);
                Program.Bin.Write(UsernameText.Text.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(UsernameText.Text));
                Program.Bin.Write(PasswordText.Text.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(PasswordText.Text));


                temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                    return;
                }

                Program.userName = UsernameText.Text;
                Program.Password = PasswordText.Text;
                Program.path = folderBrowserDialog1.SelectedPath;
                Program.ip = IpAddressText.Text;
                Program.porta = PortaText.Text;

                if (CartDaSyncButton.Visible == true)
                { // i.e. Ho effettuato la registrazione
                    try
                    {
                        using (System.IO.StreamWriter file = new System.IO.StreamWriter(@".\_" + UsernameText.Text + @"_Config_.bin", false, System.Text.Encoding.Unicode))
                        {
                            file.WriteLine(Program.path);
                        }
                    }
                    catch
                    {
                        //il file non viene creato ma non è un grande problema
                    }
                }

                this.flag = true;
                e.Result = "OK";
            }
            catch (System.IO.IOException err)
            {
                this.flag = true;
                e.Result = "Client closed";
            }
            catch (Exception err) {
                e.Result = "Err generico";
            }
        }

        private void DoWork_end(object sender, RunWorkerCompletedEventArgs e) {
            String result = (string)e.Result;
            if (result.CompareTo("Client closed") == 0)
            {
                MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
            else if (result.CompareTo("OK") == 0) {
                this.Close();
            }
            else if (result.CompareTo("Err generico") == 0)
            {
                MessageBox.Show("L'ultima operazione ha causato un errore generico inaspettato.\n Si consiglia di riprovare e nel caso di riavviare l'applicazione", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else{
                MessageBox.Show(result, "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
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
            PortaText.Visible = true;
           

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
            PortaText.Visible = true;

        }

        private void Indietro_Click(object sender, EventArgs e)
        {
            LoginButton.Visible = true;
            RegisterButton.Visible = true;
            label3.Visible = true;
            HelpButtonN.Visible = true;
            AboutButton.Visible = true;
            OkButton.Visible = false;
            IndietroButton.Visible = false;
            CartDaSyncButton.Visible = false;
            label4.Visible = false;
            infoLabel.Visible = false;
            UsernameText.Visible = false;
            PasswordText.Visible = false;
            IpAddressText.Visible = false;
            PortaText.Visible = false;
            label4.Text = "Inserisci tutti i campi";
            label4.ForeColor = System.Drawing.Color.White;
        }

        private void OK_Click(object sender, EventArgs e)
        {
            if (bw.IsBusy != true)
            {
                bw.RunWorkerAsync();
            }
            else
            {
                MessageBox.Show("Il login è in corso.\nIl programma sta cercando di contattare il server.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
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
            PortaText.Visible = false;
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
            PortaText.Visible = true;
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

        private void Porta_TextChanged(object sender, EventArgs e)
        {
            PortaText.ForeColor = SystemColors.WindowText;
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

        private void Porta_MouseClick(object sender, MouseEventArgs e)
        {
            PortaText.SelectAll();
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

            try{
                FormClosingEventArgs er = (FormClosingEventArgs) e;
                    
                    if (flag == false)
                    {
                        Program.myprocess.Kill();
                    }    
                }
                catch
                {
                    //Not a problem.. the user can manually kill it.. not nice to show the ecception
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

            string pattern = @"^[a-zA-Z0-9]+$";
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

        public bool IsValidpass(string addr)
        {

            string pattern = @"^[a-zA-Z0-9!£$%&()=?^//]+$";
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

        public bool IsValidPort(string addr)
        {

            string pattern = @"^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$";
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
