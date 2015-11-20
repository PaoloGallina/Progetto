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
using System.Text.RegularExpressions;

         

namespace PrimaGUI
{
    public partial class Form1 : Form
    {
        public AutoResetEvent T = new AutoResetEvent(false);
        private bool flag= false;
        private BackgroundWorker bw = new BackgroundWorker();

        private List<string[]> Result=new List<string[]>();
        private Object ResultLock = new Object();
    
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
            this.Text = "Ciao " + Program.userName + ", la cartella attualmente sincronizzata è " + Program.path;
        
      
            foreach (DataGridViewColumn c in dataGridView1.Columns)
            {
                c.DefaultCellStyle.Font = new Font("Times New Roman", 14F, GraphicsUnit.Pixel);
                c.DefaultCellStyle.BackColor = System.Drawing.SystemColors.ActiveCaption;
                c.DefaultCellStyle.SelectionBackColor = System.Drawing.SystemColors.ActiveCaption;
                c.DefaultCellStyle.SelectionForeColor = Color.Black;
                c.DefaultCellStyle.ForeColor = Color.Black;
           
            }
            dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            dataGridView1.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
            dataGridView1.ColumnHeadersDefaultCellStyle.BackColor = System.Drawing.SystemColors.InactiveCaption;
            dataGridView1.ColumnHeadersDefaultCellStyle.SelectionBackColor = System.Drawing.SystemColors.InactiveCaption;
            dataGridView1.ColumnHeadersDefaultCellStyle.Font = new Font("Times New Roman", 16F, GraphicsUnit.Pixel);

            bw.WorkerReportsProgress = false;
            bw.WorkerSupportsCancellation = false;
            bw.DoWork += new DoWorkEventHandler(DoWork);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(DoWork_end);
        }

        private void DoWork(object sender, DoWorkEventArgs e)
        {
            String action = (string)e.Argument;
            if (action.CompareTo("clicksync") == 0 || action.CompareTo("timersync") == 0)
            {
                SYNC_DoWork(ref e);
            }
            else if (action.CompareTo("VisualizzaUltimaVersione")==0) {
                VisualizzaUltimaVersione_DoWork(ref e);
            }
        }

        private void DoWork_end(object sender, RunWorkerCompletedEventArgs e)
        {
            String result = (string)e.Result;
            if (result.CompareTo("Client closed") == 0)
            {
                MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
            else if (result.CompareTo("Sync completed") == 0)
            {
                MessageBox.Show("La sincronizzazione è avvenuta con successo", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }else if(result.CompareTo("ShowGrid")==0){
                lock(ResultLock){
                    int index = 0;
                    foreach (String[] array in Result) {
                        if (dataGridView1.Rows.Count == index)
                        {
                            this.dataGridView1.Rows.Add();
                        }  
                        this.dataGridView1.Rows[index].Cells[0].Value = array[0];
                        this.dataGridView1.Rows[index].Cells[1].Value = array[1];
                        this.dataGridView1.Rows[index].Cells[2].Value = array[2];
                        this.dataGridView1.Rows[index].Cells[2].Value = array[3];
                        this.dataGridView1.Rows[index].Cells[4].Value = array[4];    
                        index++;  
                    }
                    this.dataGridView1.Columns[0].HeaderText = "Path";
                    this.dataGridView1.Columns[1].HeaderText = "";
                    this.dataGridView1.RowCount = Result.Count;
                    dataGridView1.Visible = true;
                }
            }
            else if (result.CompareTo("Not Show") != 0)
            {
                MessageBox.Show(result + "\nProtrebbe essere caduta la connessione o essere stato stoppato il processo server dall'amministratore", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void SYNC_Click(object sender, EventArgs e)
        {  
            if (bw.IsBusy != true)
            {
                bw.RunWorkerAsync("clicksync");
            }
            else {
                MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void SYNC_Timer(object sender, EventArgs e)
        {
            if (bw.IsBusy != true)
            {
                bw.RunWorkerAsync("timersync");
            }

        }

        private void SYNC_DoWork(ref DoWorkEventArgs e)
        {
            try
            {
                Program.Bin.Write(10);
                if (sendCred(ref e) == 999)
                    return;
                Program.Bin.Write(Program.path.Length);
                Program.Bin.Write(Encoding.Unicode.GetBytes(Program.path));
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                }
                else
                {
                    string arg = (string)e.Argument;
                    if (arg.CompareTo("clicksync") == 0)
                    {
                        e.Result = "Sync completed";
                    }
                    else
                    {
                        e.Result = "Not Show";
                    }
                }
            }
            catch (System.IO.IOException err)
            {
                this.flag = true;
                e.Result = "Client closed";
            }
            catch (Exception err)
            {
                this.flag = true;
                e.Result = "Errore Generico\n";
            }

        }

        private void VisualizzaUltimaVersione_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Visible == false)
            {
                if (bw.IsBusy != true)
                {
                    bw.RunWorkerAsync("VisualizzaUltimaVersione");
                }
                else
                {
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else { 
             dataGridView1.Visible = false;
            }
        }

        private void VisualizzaUltimaVersione_DoWork(ref DoWorkEventArgs e)
        {
            try
            {
                    int index = 0;
                    Program.Bin.Write(20);
                    if (sendCred(ref e) == 999)
                        return;

                    lock (ResultLock)
                    {
                    Result.Clear();
                    }

                    while (true)
                    {
                        Program.Sr.DiscardBufferedData();
                        string patht = Program.Sr.ReadLine();
                        if (patht.CompareTo(@"end") == 0)
                        {
                           break;
                        }
                        else if (patht.CompareTo(@"ERRORE OP. NON EFFETTUATA") == 0)
                        {
                            e.Result = patht;
                            return;
                        }
                        
                            Program.Sr.DiscardBufferedData();
                            string last = Program.Sr.ReadLine();
                            Program.Srchar.DiscardBufferedData();
                            string hash = Program.Srchar.ReadLine();
                        
                        lock (ResultLock){
                        Result.Add(new string[] { patht, hash, last, "", "Restore" });}

                        index++;
                    }

                    Program.Sr.DiscardBufferedData();
                    string temp = Program.Sr.ReadLine();
                    if (temp.CompareTo("OK") != 0)
                    {
                        e.Result = Text;
                    }
                    else {
                        e.Result = "ShowGrid";
                    }
                
            }
            catch (System.IO.IOException err)
            {
                this.flag = true;
                e.Result = "Client closed";
            }
        }

        private void VisualizzaFile_Click(object sender, EventArgs e)
        {
            try { 
            label1.Text = "";
            if (dataGridView1.Visible == false){

                int index = 0;
                Program.Bin.Write(60);
                if (sendCred() == 999)
                    return;
                
                while (true){
                    Program.Sr.DiscardBufferedData();
                    string patht = Program.Sr.ReadLine();
                    if (patht.CompareTo(@"end") == 0){
                        this.dataGridView1.Columns[0].HeaderText = "Path";
                        this.dataGridView1.Columns[1].HeaderText = "";
                        this.dataGridView1.RowCount = index;
                        dataGridView1.Visible = true;
                        break;
                    }
                    else if (patht.CompareTo(@"ERRORE OP. NON EFFETTUATA") == 0)
                    {
                        label1.Text = patht;
                        return;
                    }
                    Program.Sr.DiscardBufferedData();
                    string last = Program.Sr.ReadLine();
                    Program.Srchar.DiscardBufferedData();
                    string hash = Program.Srchar.ReadLine();
                    if (dataGridView1.Rows.Count == index){
                        this.dataGridView1.Rows.Add();
                    }
                    this.dataGridView1.Rows[index].Cells[0].Value = patht;
                    this.dataGridView1.Rows[index].Cells[1].Value = hash;
                    this.dataGridView1.Rows[index].Cells[2].Value = last;
                    
                    this.dataGridView1.Rows[index].Cells[4].Value = "Restore";
                    index++;
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0){
                    label1.Text = temp;}
            }
            else{
                dataGridView1.Visible = false;
            }
            }
            catch (System.IO.IOException err)
            {

                this.flag = true;
                MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
        }

        private void VisualizzaVersioni_Click(object sender, EventArgs e)
        {
            try { 
            label1.Text = "";
            if (dataGridView1.Visible == false)
            {
                int index = 0;
                Program.Bin.Write(70);
                if (sendCred() == 999)
                    return;
                
                while (true)
                {
                    Program.Srchar.DiscardBufferedData();
                    string numero = Program.Srchar.ReadLine();
                    if (numero.CompareTo(@"end") == 0)
                    {
                        this.dataGridView1.Columns[0].HeaderText = "Versione numero";
                        this.dataGridView1.Columns[1].HeaderText = "Data Creazione";
                        this.dataGridView1.RowCount = index;
                        dataGridView1.Visible = true;
                        break;
                    }
                    else if (numero.CompareTo(@"ERRORE OP. NON EFFETTUATA") == 0)
                    {
                        label1.Text = numero;
                        return;
                    }
                    Program.Srchar.DiscardBufferedData();
                    string data = Program.Srchar.ReadLine();

                    if (dataGridView1.Rows.Count == index)
                    {
                        this.dataGridView1.Rows.Add();
                    }
                    
                    this.dataGridView1.Rows[index].Cells[4].Value = "Visualizza";

                    this.dataGridView1.Rows[index].Cells[0].Value = numero;
                    this.dataGridView1.Rows[index].Cells[2].Value = data;
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
            catch (System.IO.IOException err)
            {

                this.flag = true;
                MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            try { 
            label1.Text = "";
            if (e.ColumnIndex != 4)
            {
                return;
            }
            if (this.dataGridView1.Columns[0].HeaderText.CompareTo("Versione numero") != 0)
            {
                Program.Bin.Write(50);
                if (sendCred() == 999)
                    return;


                int index = e.RowIndex;
                string path = (string)this.dataGridView1.Rows[index].Cells[0].Value;
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
            else
            {
                int index = 0;
                Program.Bin.Write(80);
                if (sendCred() == 999)
                    return;

                int ver = Int32.Parse((string)this.dataGridView1.Rows[e.RowIndex].Cells[0].Value);
                Program.Bin.Write(ver);

                while (true)
                {
                    Program.Sr.DiscardBufferedData();
                    string patht = Program.Sr.ReadLine();
                    if (patht.CompareTo(@"end") == 0)
                    {
                        this.dataGridView1.Columns[0].HeaderText = "Path";
                        this.dataGridView1.Columns[1].HeaderText = "";
                        this.dataGridView1.RowCount = index;
                        break;
                    }
                    else if (patht.CompareTo(@"ERRORE OP. NON EFFETTUATA") == 0)
                    {
                        label1.Text = patht;
                        return;
                    }
                    Program.Sr.DiscardBufferedData();
                    string last = Program.Sr.ReadLine();
                   
                    Program.Srchar.DiscardBufferedData();
                    string hash = Program.Srchar.ReadLine();

                    if (dataGridView1.Rows.Count == index)
                    {
                        this.dataGridView1.Rows.Add();
                    }
                    this.dataGridView1.Rows[index].Cells[0].Value = patht;
                    this.dataGridView1.Rows[index].Cells[1].Value = hash;
                    this.dataGridView1.Rows[index].Cells[4].Value = "Restore";
                    this.dataGridView1.Rows[index].Cells[2].Value = last;
                    index++;
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    label1.Text = temp;
                }

            }
            }
            catch (System.IO.IOException err)
            {

                this.flag = true;
                MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
        }

        private void SelectPath_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            if (folderBrowserDialog1.SelectedPath != "")
            {
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
                this.Text = "Ciao " + Program.userName + ", la cartella attualmente sincronizzata è " + Program.path;
            }
        }

        private int sendCred()
        {
            try { 
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

            Program.Sr.DiscardBufferedData();
            temp = Program.Sr.ReadLine();
            if (temp.CompareTo("OK") != 0)
            {
                label1.Text = temp;
                label1.ForeColor = System.Drawing.Color.Red;
                return 999;
            }
            }
            catch (System.IO.IOException err)
            {

                this.flag = true;
                MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                this.Close();
            }
            return 0;
        }

        private int sendCred(ref DoWorkEventArgs e)
        {
            try
            {
                Program.Bin.Write(Program.ip.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(Program.ip));
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                    return 999;
                }
                Program.Bin.Write(Program.userName.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(Program.userName));
                Program.Bin.Write(Program.Password.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(Program.Password));

                Program.Sr.DiscardBufferedData();
                temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                    return 999;
                }
            }
            catch (System.IO.IOException err)
            {
                this.flag = true;
                e.Result = "Client closed";
                return 999;
            }
            return 0;
        }

        //Necessario per la chiusura del processo client C++
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            if (flag == false)
                Program.myprocess.Kill();
           
                base.OnClosing(e);
            
        }

    }
}
