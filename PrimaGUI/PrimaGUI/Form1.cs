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
            try
            {
                using (System.IO.StreamReader file = new System.IO.StreamReader(@".\_" + Program.userName + @"_Config_.bin", System.Text.Encoding.Unicode))
                {
                    Program.path=file.ReadLine();
                }
            }
            catch
            {
                //il file non è presente, ma non è grave
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
                    file.WriteLine(Program.path);
                }
            }
            catch
            {
                //il file non è presente, ma non è grave l'utente dovrà caricarl ogni volta
            }
            this.Text = "Ciao, " + Program.userName + " la cartella attualmente sincronizzata è " + Program.path;
        
      
            foreach (DataGridViewColumn c in dataGridView1.Columns)
            {
                c.DefaultCellStyle.Font = new Font("Times New Roman", 14F, GraphicsUnit.Pixel);
                c.DefaultCellStyle.BackColor = System.Drawing.SystemColors.ActiveCaption;
                c.DefaultCellStyle.SelectionBackColor = System.Drawing.SystemColors.ActiveCaption;
                c.DefaultCellStyle.SelectionForeColor = Color.Black;
                c.DefaultCellStyle.ForeColor = Color.Black;
            }
            dataGridView1.AutoSize = true;
            dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            dataGridView1.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
            dataGridView1.ColumnHeadersDefaultCellStyle.BackColor = System.Drawing.SystemColors.InactiveCaption;
            dataGridView1.ColumnHeadersDefaultCellStyle.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            dataGridView1.ColumnHeadersDefaultCellStyle.Font = new Font("Times New Roman", 16F, GraphicsUnit.Pixel);
            
        }

        private void VisualizzaVersione_Click(object sender, EventArgs e)
        {
           
            if (dataGridView1.Visible == false)
            {
                dataGridView1.Visible = true;
                int index = 0;
                Program.Bin.Write(20);
                if (sendCred() == 999) {
                    return;
                }

                while (true)
                {
                    Program.Sr.DiscardBufferedData();
                    string patht = Program.Sr.ReadLine();
                    if (patht.CompareTo(@"end") == 0) {
                        this.dataGridView1.RowCount=index;
                        break;
                    }
                    Program.Srchar.DiscardBufferedData();
                    string hash = Program.Srchar.ReadLine();

                    if (dataGridView1.Rows.Count == index){
                        this.dataGridView1.Rows.Add();
                    }
                    this.dataGridView1.Rows[index].Cells[0].Value = patht;
                    this.dataGridView1.Rows[index].Cells[1].Value = hash;
                    this.dataGridView1.Rows[index].Cells[3].Value = "Restore"; 
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
            this.Text = "Ciao, " + Program.userName + " la cartella attualmente sincronizzata è " + Program.path;
            
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

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
           
            if (e.ColumnIndex != 3)
            {
                return;
            }
            Program.Bin.Write(50);
            if (sendCred() == 999)
            {
                return;
            }
            int index = e.RowIndex;
            string path =(string) this.dataGridView1.Rows[index].Cells[0].Value;
            string hash = (string)this.dataGridView1.Rows[index].Cells[1].Value;
            hash.ToLower();
            Program.Bin.Write(path.Length);
            Program.Bin.Write(Encoding.Unicode.GetBytes(path));
            Program.Bin.Write(hash.Length);
            Program.Bin.Write(Encoding.ASCII.GetBytes(hash));

            Program.Sr.DiscardBufferedData();
            string temp = Program.Sr.ReadLine();
            if (temp.CompareTo("OK") != 0)
            {
                label1.Text = temp;
            }
        }
    }
}
