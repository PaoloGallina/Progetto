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
    public partial class FinestraPrincipale : Form
    {
        public AutoResetEvent T = new AutoResetEvent(false);
        private bool clientclosed= false;
        private bool operationflag = true;

        public BackgroundWorker bw = new BackgroundWorker();
        private List<string[]> Result=new List<string[]>();
        private String path;
        private String hash;
  
        public FinestraPrincipale()
        {
            InitializeComponent();
    
        }

        private void DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                String action = (string)e.Argument;
                if (action.CompareTo("clicksync") == 0 || action.CompareTo("timersync") == 0)
                {
                    SYNC_DoWork(sender,ref e);
                }
                else if (action.CompareTo("VisualizzaUltimaVersione") == 0)
                {
                    RipristinaUltimaVersione_DoWork(ref e);
                }
                else if (action.CompareTo("VisualizzaFile") == 0)
                {
                    VisualizzaFile_DoWork(ref e);
                }
                else if (action.CompareTo("VisualizzaVersione") == 0)
                {
                    VisualizzaVersione_DoWork(ref e);
                }
                else if (action.CompareTo("VisualizzaVersioni") == 0)
                {
                    VisualizzaVersioni_DoWork(ref e);
                }
                else if (action.CompareTo("Restore") == 0)
                {
                    Restore_DoWork(ref e);
                }
            }
            catch (Exception errs)
            {
                e.Result = "Errore Generico";
            }
        }

        private void DoWork_end(object sender, RunWorkerCompletedEventArgs e)
        {
            try
            {
                #region "Gestion risultati Dowork"
                String result = (string)e.Result;
                if (result.CompareTo("Client closed") == 0)
                {
                    MessageBox.Show("Non devi mai chiudere il client C++, continuare l'esecuzione è impossibile e il programma terminerà, ma nessun dato è andato perso.\n L'utente potrà effettuare il login eseguendo nuovamente il programma.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    this.Close();
                }
                else if (result.CompareTo("Restore avvenuto con successo") == 0)
                {
                    MessageBox.Show(result, "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else if (result.CompareTo("Sync completed") == 0)
                {
                    MessageBox.Show("La sincronizzazione è avvenuta con successo", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else if (result.CompareTo("La sync non era necessaria") == 0)
                {
                    MessageBox.Show("La sincronizzazione non è avvenuta poiché la copia di backup è gia aggiornata.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else if (result.CompareTo("ACCESS ERRORE OP. NON EFFETTUATA") == 0)
                {
                    MessageBox.Show("La sincronizzazione non è avvenuta con successo poiché il programma non riesce ad accedere ad un file aperto in un altro processo.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else if (result.CompareTo("Errore Generico") == 0)
                {
                    MessageBox.Show("L'ultima operazione ha causato un errore generico inaspettato, nessun dato è andato perso, ma l'operazione non si è conclusa correttamente.\n Si consiglia di riprovare e nel caso di riavviare l'applicazione", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else if (result.CompareTo("Rest Succ") == 0)
                {
                    MessageBox.Show("Il restore completo dell'ultima versione presente sul server è avvenuta con successo", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);

                }
                else if (result.CompareTo("Sei già loggato da un altro terminale. ERRORE OP. NON EFFETTUATA") == 0)
                {
                    MessageBox.Show("Sei già loggato da un altro terminale", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);

                }
                else if (result.CompareTo("ShowGrid") == 0)
                {
                    
                        int index = 0;
                        foreach (String[] array in Result)
                        {
                            if (dataGridView1.Rows.Count == index)
                            {
                                this.dataGridView1.Rows.Add();
                            }
                            this.dataGridView1.Rows[index].Cells[0].Value = array[0];
                            this.dataGridView1.Rows[index].Cells[1].Value = array[1];
                            this.dataGridView1.Rows[index].Cells[2].Value = "    " + array[2] + "    ";
                            this.dataGridView1.Rows[index].Cells[3].Value = array[3];
                            this.dataGridView1.Rows[index].Cells[4].Value = array[4];
                            index++;
                        }

                        this.dataGridView1.Columns[0].HeaderText = "Path";
                        this.dataGridView1.Columns[1].HeaderText = "";
                        this.dataGridView1.Columns[2].HeaderText = "Data Ultima Modifica";
                        this.dataGridView1.RowCount = Result.Count;                 
                        dataGridView1.Visible = true;
                        dataGridView2.Visible = false;
                        dataGridView3.Visible = false;
                }
                else if (result.CompareTo("ShowGrid2") == 0)
                {
                        
                        int index = 0;
                        foreach (String[] array in Result)
                        {
                            if (dataGridView2.Rows.Count == index)
                            {
                                this.dataGridView2.Rows.Add();
                            }
                            this.dataGridView2.Rows[index].Cells[0].Value = array[0];
                            this.dataGridView2.Rows[index].Cells[1].Value = array[1];
                            this.dataGridView2.Rows[index].Cells[2].Value = "    " + array[2] + "    ";
                            this.dataGridView2.Rows[index].Cells[3].Value = array[3];
                            this.dataGridView2.Rows[index].Cells[4].Value = array[4];
                            index++;
                        }

                        this.dataGridView2.Columns[0].HeaderText = "Numero Versione";
                        this.dataGridView2.Columns[1].HeaderText = "";
                        this.dataGridView2.Columns[2].HeaderText = "Data Creazione";
                        this.dataGridView2.RowCount = Result.Count;
                        dataGridView2.Visible = true;
                        dataGridView1.Visible = false;
                        dataGridView3.Visible = false;
                }
                else if (result.CompareTo("ShowGrid3") == 0)
                {

                    int index = 0;
                    foreach (String[] array in Result)
                    {
                        if (dataGridView3.Rows.Count == index)
                        {
                            this.dataGridView3.Rows.Add();
                        }
                        this.dataGridView3.Rows[index].Cells[0].Value = array[0];
                        this.dataGridView3.Rows[index].Cells[1].Value = array[1];
                        this.dataGridView3.Rows[index].Cells[2].Value = "    " + array[2] + "    ";
                        this.dataGridView3.Rows[index].Cells[3].Value = array[3];
                        this.dataGridView3.Rows[index].Cells[4].Value = array[4];
                        index++;
                    }

                    this.dataGridView3.Columns[0].HeaderText = "Path";
                    this.dataGridView3.Columns[1].HeaderText = "";
                    this.dataGridView3.Columns[2].HeaderText = "Data Ultima Modifica";
                    this.dataGridView3.RowCount = Result.Count;
                    dataGridView3.Visible = true;
                    dataGridView2.Visible = false;
                    dataGridView1.Visible = false;
                }
                else if (result.CompareTo("Not Show") != 0)
                {
                    MessageBox.Show(result + "\nProtrebbe essere caduta la connessione o essere stato stoppato il processo server dall'amministratore", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                operationflag = true;
                #endregion
            }
            catch (Exception err)
            {
                operationflag = true;
                MessageBox.Show("L'ultima operazione ha causato un errore generico inaspettato, nessun dato è andato perso, ma l'operazione non si è conclusa correttamente.\n Si consiglia di riprovare e nel caso di riavviare l'applicazione", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
               
        }

        private void bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.progress.Text = (e.ProgressPercentage.ToString()+"%");
        }

        //Metodi Eventi
        private void SYNC_Click(object sender, EventArgs e)
        {  
            if (bw.IsBusy != true && operationflag==true)
            {
                operationflag = false;
                dataGridView1.Visible = false;
                dataGridView2.Visible = false;
                dataGridView3.Visible = false;
                bw.RunWorkerAsync("clicksync");
            }
            else {
                MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void SYNC_Timer(object sender, EventArgs e)
        {
            if (bw.IsBusy != true && operationflag == true)
            {
                operationflag = false;
                dataGridView1.Visible = false;
                dataGridView2.Visible = false;
                dataGridView3.Visible = false;
                bw.RunWorkerAsync("timersync");
            }

        }

        private void RipristinaUltimaVersione_Click(object sender, EventArgs e)
        {

            if (bw.IsBusy != true && operationflag == true)
                {
                    operationflag = false;
                    bw.RunWorkerAsync("VisualizzaUltimaVersione");
                }
                else
                {
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            
        }

        private void VisualizzaFile_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Visible == false)
            {
                if (bw.IsBusy != true && operationflag == true)
                {
                    operationflag = false;
                    bw.RunWorkerAsync("VisualizzaFile");
                }
                else
                {
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                dataGridView1.Visible = false;
            }
        }

        private void VisualizzaVersioni_Click(object sender, EventArgs e)
        {
            if (dataGridView2.Visible == false)
            {
                if (bw.IsBusy != true && operationflag == true)
                {
                    operationflag = false;
                    bw.RunWorkerAsync("VisualizzaVersioni");
                }
                else
                {
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                dataGridView2.Visible = false;
            }
        }

        private void dataGridView_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

            if (e.ColumnIndex != 4)
                return;
            
            if (this.dataGridView1.Visible == true)
            {
                if (bw.IsBusy != true && operationflag == true)
                {
                    operationflag = false;
                    path = (string)this.dataGridView1.Rows[e.RowIndex].Cells[0].Value;
                    hash = (string)this.dataGridView1.Rows[e.RowIndex].Cells[1].Value;
                    hash.ToLower();
                    bw.RunWorkerAsync("Restore");
                }
                else
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else if ( this.dataGridView3.Visible == true)
            {
                if (bw.IsBusy != true && operationflag == true)
                {
                    operationflag = false;
                    path = (string)this.dataGridView3.Rows[e.RowIndex].Cells[0].Value;
                    hash = (string)this.dataGridView3.Rows[e.RowIndex].Cells[1].Value;
                    hash.ToLower();
                    bw.RunWorkerAsync("Restore");
                }
                else
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                if (bw.IsBusy != true && operationflag == true)
                {
                    operationflag = false;
                    path = (string)this.dataGridView2.Rows[e.RowIndex].Cells[0].Value;
                    bw.RunWorkerAsync("VisualizzaVersione");
                }
                else
                    MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

        }

        private void SelectPath_Click(object sender, EventArgs e)
        {
            if (bw.IsBusy != true && operationflag == true)
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
                        MessageBox.Show("Il programma non è riuscito a creare il file per salvare le impostazioni per i login futuri. \nNon è un problema, tutto funzionarà correttamente.\nAl prossimo avvio prova a risolvere il problema eseguendo come amministratore.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    this.Text = "Ciao " + Program.userName + ", la cartella attualmente sincronizzata è " + Program.path;
                }
            }
            else
                MessageBox.Show("Una operazione è già in corso.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);

        }

        //Metodi DoWork 
        private void SYNC_DoWork(object sender,ref DoWorkEventArgs e)
        {
            try
            {
                BackgroundWorker worker = sender as BackgroundWorker; 

                Program.Bin.Write(10);
                Program.Bin.Write(Program.path.Length);
                Program.Bin.Write(Encoding.Unicode.GetBytes(Program.path));
                
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                    string arg = (string)e.Argument;
                    if (arg.CompareTo("clicksync") != 0 && temp.CompareTo("La sync non era necessaria") == 0)
                    {
                        e.Result = "Not Show";
                    }
                    return;
                }

                if (sendCred(ref e) == 999)
                {
                    return;
                }
                Program.Bin.Write(Program.path.Length);
                Program.Bin.Write(Encoding.Unicode.GetBytes(Program.path));

                worker.ReportProgress(0);   
                Program.Sr.DiscardBufferedData();
                temp = Program.Sr.ReadLine();
                int totale = Int32.Parse(temp);
                int n=0;

                while (temp.CompareTo("end") != 0)
                {
                    Program.Sr.DiscardBufferedData();
                    temp = Program.Sr.ReadLine();
                    if (temp.CompareTo("end") != 0 && temp.CompareTo("prog") != 0)
                    {
                        worker.ReportProgress(0);
                        e.Result = temp;
                        return;
                    }
                    else
                    {
                        n++;
                        if (n == totale)
                        {
                            worker.ReportProgress(100);
                        }
                        else {
                            worker.ReportProgress(100*n/totale); 
                        }
                    }
                }


                Program.Sr.DiscardBufferedData();
                temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    worker.ReportProgress(0);
                    string arg = (string)e.Argument;
                    if (arg.CompareTo("clicksync") == 0)
                    {
                        e.Result = temp;
                    }
                }
                else
                {
                    string arg = (string)e.Argument;
                    if (arg.CompareTo("clicksync") == 0)
                    {
                        worker.ReportProgress(100);
                        e.Result = "Sync completed";
                    }
                    else
                    {
                        worker.ReportProgress(100);
                        e.Result = "Not Show";
                    }
                }
            }
            catch (System.IO.IOException err)
            {
                this.clientclosed = true;
                e.Result = "Client closed";
            }
            catch (Exception err)
            {
                
                e.Result = "Errore Generico\n";
            }

        }

        private void VisualizzaFile_DoWork(ref DoWorkEventArgs e)
        {
            try
            {
                Program.Bin.Write(60);
                if (sendCred(ref e) == 999){
                    return;
                }

                Result.Clear();

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
                    string ultimamodifica = Program.Sr.ReadLine();
                    Program.Srchar.DiscardBufferedData();
                    string hash = Program.Srchar.ReadLine();

                    Result.Add(new string[] { patht, hash, ultimamodifica, "", "Restore" });
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                }
                else
                {
                    e.Result = "ShowGrid";
                }
            }
            catch (System.IO.IOException err)
            {
                this.clientclosed = true;
                e.Result = "Client closed";
            }
            catch (Exception err) {
                e.Result = "Errore Generico";
            }
        }

        private void RipristinaUltimaVersione_DoWork(ref DoWorkEventArgs e)
        {
            try
            {
                Program.Bin.Write(20);
                if (sendCred(ref e) == 999)
                    return;

                Result.Clear();

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
                    Result.Add(new string[] { patht, hash, last, "", "Restore" });
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                    return;
                }
                else
                {
                    e.Result = "Rest Succ";
                }

                foreach (String[] array in Result)
                {
                    path = array[0];
                    hash = array[1];
                    hash.ToLower();
                    Restore_DoWork(ref e);
                    String result = (string)e.Result;
                    if (result.CompareTo("Restore avvenuto con successo") != 0)
                    {
                        e.Result = "Errore durante il restore di un file";
                        return;
                    }
                }
                e.Result = "Rest Succ";

            }
            catch (System.IO.IOException err)
            {
                this.clientclosed = true;
                e.Result = "Client closed";
            }
            catch (Exception err)
            {
                e.Result = "Errore Generico";
            }
        }

        private void VisualizzaVersione_DoWork(ref DoWorkEventArgs e)
        {
            try
            {       
                Result.Clear();
                Program.Bin.Write(80);
                if (sendCred(ref e) == 999)
                    return;
                int ver;
                ver = Int32.Parse(path);
                Program.Bin.Write(ver);

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
                   
                        Result.Add(new string[] { patht, hash, last, "", "Restore" });
               
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                }
                else
                {
                    e.Result = "ShowGrid3";
                }

            }
            catch (System.IO.IOException err)
            {
                this.clientclosed = true;
                e.Result = "Client closed";
            }
            catch (Exception err) {
                e.Result = "Errore Generico";
            }

        }

        private void VisualizzaVersioni_DoWork(ref DoWorkEventArgs e)
        {
            try { 

                Program.Bin.Write(70);
                if (sendCred(ref e) == 999)
                    return;
               
                    Result.Clear();
                while (true)
                {
                    Program.Srchar.DiscardBufferedData();
                    string numero = Program.Srchar.ReadLine();
                    if (numero.CompareTo(@"end") == 0){
                        break;
                    }
                    else if (numero.CompareTo(@"ERRORE OP. NON EFFETTUATA") == 0)
                    {
                        e.Result = numero;
                        return;
                    }
                    Program.Srchar.DiscardBufferedData();
                    string data = Program.Srchar.ReadLine();
                   
                        Result.Add(new string[] { numero, "", data, "", "Visualizza" });
                }
                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                }
                else
                {
                    e.Result = "ShowGrid2";
                }
            
            }
            catch (System.IO.IOException err)
            {
                this.clientclosed = true;
                e.Result = "Client closed";
            }
            catch (Exception err) {
                e.Result = "Errore Generico";
            }

        }

        private void Restore_DoWork(ref DoWorkEventArgs e)
        {
            try
            {
                Program.Bin.Write(50);
                if (sendCred(ref e) == 999)
                    return;

                Program.Bin.Write(path.Length);
                Program.Bin.Write(Encoding.Unicode.GetBytes(path));
                Program.Bin.Write(hash.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(hash));

                Program.Sr.DiscardBufferedData();
                string temp = Program.Sr.ReadLine();
                if (temp.CompareTo("OK") != 0)
                {
                    e.Result = temp;
                }
                else
                {
                    e.Result = "Restore avvenuto con successo";
                }
            }
            catch (System.IO.IOException err)
            {
                this.clientclosed = true;
                e.Result = "Client closed";
            }
            catch (Exception err) {
                e.Result = "Errore Generico";
            }
        }

        //Necessario per la chiusura del processo client C++
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            if (clientclosed == false)
            {
                try
                {
                    Program.myprocess.Kill();
                }
                catch
                {
                    MessageBox.Show("Il programma ha incontrato un problema nella chiusura del client c++. Procesi manualmente alla chiusura", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
                base.OnClosing(e);
            
        }

        //Altro
        private int sendCred(ref DoWorkEventArgs e)
        {
            try
            {
                Program.Bin.Write(Program.ip.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(Program.ip));
                Program.Bin.Write(Program.porta.Length);
                Program.Bin.Write(Encoding.ASCII.GetBytes(Program.porta));
                
                
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
                this.clientclosed = true;
                e.Result = "Client closed";
                return 999;
            }
            catch (Exception err) {
                e.Result = "Errore Generico";
                return 999;
            }
            return 0;
        }

        private void SYNC_MouseEnter(object sender, EventArgs e)
        {
            progress.BackColor = System.Drawing.SystemColors.ActiveCaption;
        }

        private void SYNC_MouseLeave(object sender, EventArgs e)
        {
            progress.BackColor = System.Drawing.Color.Transparent;
        }



    }
}
