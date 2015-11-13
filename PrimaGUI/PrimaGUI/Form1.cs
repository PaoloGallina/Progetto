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
        private bool _dragging = false;
        private Point _start_point = new Point(0, 0);
        public Form1()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.None;
            try
            {
                using (System.IO.StreamReader file = new System.IO.StreamReader(@".\_" + Program.userName + @"_Config_.bin", System.Text.Encoding.Unicode))
                {
                    Program.path=file.ReadLine();
                }
            }
            catch
            {
                while (Program.path.CompareTo(@"") == 0)
                {
                    folderBrowserDialog1.Description = "File di configurazione non trovato.\nPer favore scegli nuovamente la cartella da sincronizzare.";
                    folderBrowserDialog1.ShowDialog();
                    Program.path = folderBrowserDialog1.SelectedPath;
                }
            }
            while (Program.path.CompareTo(@"") == 0)
            {
                folderBrowserDialog1.Description = "File di configurazione non trovato.\nPer favore scegli nuovamente la cartella da sincronizzare.";
                folderBrowserDialog1.ShowDialog();
                Program.path = folderBrowserDialog1.SelectedPath;
            }
            try
            {
                using (System.IO.StreamWriter file = new System.IO.StreamWriter(@".\_" + Program.userName + @"_Config_.bin", false, System.Text.Encoding.Unicode))
                {
                    file.WriteLine(folderBrowserDialog1.SelectedPath);
                }
            }
            catch
            {
                //Il file non viene creato, non è un gran problema, l'utente dovrà semplicemente immettere ogni volta 
                //le credenziali
            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
           

            if (dataGridView1.Visible == false)
            {
                dataGridView1.Visible = true;
                int index = 0;
                Program.Sr.DiscardBufferedData();
                Program.Bin.Write(20);
                if (sendCred() == 999) {
                    return;
                }
                Program.Sr.DiscardBufferedData();
                while (true)
                {
                    string patht = Program.Sr.ReadLine();
                    if (patht.CompareTo(@"end") == 0) {
                        break;
                    }
                   
                    this.dataGridView1.Rows.Add();
                    this.dataGridView1.Rows[index].Cells[0].Value = patht;
                    index++;
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    label1.Text = temp;
                }
            }
            else
            {
                dataGridView1.Visible = false;
            }


        }

        private void button2_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            Program.path = folderBrowserDialog1.SelectedPath;
            try
            {
                using (System.IO.StreamWriter file = new System.IO.StreamWriter(@".\_" + Program.userName + @"_Config_.bin", false, System.Text.Encoding.Unicode))
                {
                    file.WriteLine(folderBrowserDialog1.SelectedPath);
                }
            }
            catch
            {
                //Il file non viene creato, non è un gran problema, l'utente dovrà semplicemente immettere ogni volta 
                //le credenziali
            }
        }

        //Bisogna fare un controllo sulla buona riuscita della sync e magari far apparire un messaggio fino alla terminazione della stessa
        private void SYNC_Click(object sender, EventArgs e)
        {
            
            Program.Bin.Write(10);
            if (sendCred() == 999)
            {
                return;
            }
            Program.Bin.Write(Program.path.Length);
            Program.Bin.Write(Encoding.Unicode.GetBytes(Program.path));

            Program.Sr.DiscardBufferedData();
            string temp = Program.Sr.ReadLine();
            if (temp.CompareTo("OK") != 0)
            {
                label1.Text = temp;
            }
        }

        //Necessario per la chiusura del processo client C++
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            Program.myprocess.Kill();
            base.OnClosing(e);
        }

        //Metodi per rendere spostabile la finestra
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

        private int sendCred() {

            Program.Bin.Write(Program.ip.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(Program.ip));
            Program.Sr.DiscardBufferedData();
            string temp = Program.Sr.ReadLine();
            if (temp.CompareTo("OK") != 0)
            {
                label1.Text = temp;
                label1.ForeColor = System.Drawing.Color.Red;
                return 999;
            }
            Program.Bin.Write(Program.userName.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(Program.userName));
            Program.Bin.Write(Program.Password.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(Program.Password));

            return 0;
        }
    }
}
