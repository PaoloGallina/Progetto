using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace PrimaGUI
{
    static class Program
    {
        public static string userName="";
        public static string Password="";
        public static string path="";

        
        [STAThread]
        static void Main()
        {
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
