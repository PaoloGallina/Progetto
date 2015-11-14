using System;
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
        public static Process myprocess;
        
        [STAThread]
        static void Main()
        {
           
           
            //exception proble.. not hardcoded string
            Random rnd = new Random();
            int pipenumber = rnd.Next(10000);
         //   string pipename = "PIPE" + pipenumber;
            string pipename = "PIPE" + 1;//DEBUG
            
            myprocess = new Process();
            myprocess.StartInfo.FileName = "C:\\Users\\Paolo\\Desktop\\Progetto\\FunzioneMain\\Debug\\FunzioneMain.exe";
            myprocess.StartInfo.CreateNoWindow = true;
            myprocess.StartInfo.Arguments = pipenumber.ToString();
          //  myprocess.Start();

            while (PServer1 == null||Bin==null || Sr == null)
            {
                PServer1 = new NamedPipeServerStream(pipename, System.IO.Pipes.PipeDirection.InOut);
                PServer1.WaitForConnection();
                Sr = new StreamReader(PServer1, System.Text.Encoding.Unicode);
                Srchar = new StreamReader(PServer1, System.Text.Encoding.ASCII);
                Bin = new BinaryWriter(PServer1);
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Application.Run(new Form2());
           
            if (userName != "")
            {
                Application.Run(new Form1());
            }
            
        }
    }
}
