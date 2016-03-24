using System;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Pipes;
using System.Diagnostics;
using System.ComponentModel;

namespace PrimaGUI
{
    static class Program
    {
        public static NamedPipeServerStream PServer1 = null;
        public static StreamReader Sr = null;
        public static StreamReader Srchar = null;
        public static BinaryWriter Bin = null;
        public static string userName="";
        public static string Password="";
        public static string path="";
        public static string ip = "";
        public static string porta = "";
        public static bool LoginPerformed = false;
        public static Process myprocess;

        [STAThread]
        static void Main()
        {
            Random rnd = new Random();
            int pipenumber = rnd.Next(10000);
            string pipename = "PIPE" + pipenumber;
            //string pipename = "PIPE" + 1;//DEBUG
            try{
                myprocess = new Process();
                myprocess.StartInfo.FileName = ".\\FunzioneMain.exe";
                myprocess.StartInfo.CreateNoWindow = true;
                myprocess.StartInfo.Arguments = pipenumber.ToString();
                myprocess.StartInfo.Verb = "runas";
                myprocess.Start();
            }catch(Exception e){
                MessageBox.Show("E' stato riscontrato un errore all'avvio, l'utente si assicuri che FunzioneMain.exe sia nella stessa cartella di GEMS.exe e di eseguire come amministratore.", "Informazione per l'utente", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            while (PServer1 == null)
            {
                PServer1 = new NamedPipeServerStream(pipename, System.IO.Pipes.PipeDirection.InOut);
                PServer1.WaitForConnection();
                Sr = new StreamReader(PServer1, System.Text.Encoding.Unicode);
                Srchar = new StreamReader(PServer1, System.Text.Encoding.ASCII);
                Bin = new BinaryWriter(PServer1);
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            Application.Run(new FinestraLogin());
            if (LoginPerformed == false)
            {
                return;
            }    
            Application.Run(new FinestraPrincipale());
            
        }

    }
   
}
