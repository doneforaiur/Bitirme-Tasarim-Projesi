using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace InterfaceProject
{
    public partial class Form1 : Form
    {
        string path;
        string loc = System.IO.Path.GetDirectoryName(Application.ExecutablePath) + "/config.txt";
        System.IO.StreamWriter sw;
    //    string text1, text2, text3, text4, yer;
        public Form1()
        {
            InitializeComponent();
                            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            checkBox1.Enabled = false;
            string fLoc = "";
            fLoc = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            fLoc = fLoc + "/EVM.exe";
            fLoc.Replace("/", "//");
           

            System.Diagnostics.Process process = new System.Diagnostics.Process();
            System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();
            startInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            startInfo.FileName = "CMD.exe";
            startInfo.Arguments ="/C" + fLoc;
            process.StartInfo = startInfo;
            process.Start();

            /*
            string strCmdText;
            strCmdText = "/C C:\\Users\\Toshiba\\Desktop\\OpenCVProje\\OpenCV\\x64\\Release\\OpenCV.exe";
            System.Diagnostics.Process.Start("CMD.exe", strCmdText);
            Process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            */
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            string okunan = textBox1.Text;
            okunan.Replace("/", "//");
            textBox1.Text = okunan;
        }

        private void button2_Click(object sender, EventArgs e) 
        {
            System.Diagnostics.Process[] localByName = System.Diagnostics.Process.GetProcessesByName("EVM");
            foreach (System.Diagnostics.Process p in localByName)
            {
                p.Kill();
            }
            checkBox1.Enabled = true;
            if (System.IO.File.Exists(loc))
            {
                System.IO.File.Delete(loc);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string oSelectedFile = "";
            System.Windows.Forms.OpenFileDialog oDlg = new System.Windows.Forms.OpenFileDialog();
            if (System.Windows.Forms.DialogResult.OK == oDlg.ShowDialog())
            {
                oSelectedFile = oDlg.FileName;
                path = System.IO.Path.GetFileName(oDlg.FileName);
                path = path.Remove(path.Length - 4);
                textBox1.Text = oSelectedFile;
                
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            
            float deger;
            deger = trackBar1.Value;
            deger = deger / 10;
            label1.Text = deger.ToString();
           
      /*      text1 = System.IO.File.ReadAllText(loc);
            text1 = string.Format(text1.Replace(System.IO.File.ReadLines(loc).ElementAt(0), label1.Text.ToString()));
            System.IO.File.WriteAllText(loc, text1);
      */
            //    using (sw = System.IO.File.AppendText(loc))
            //       sw.WriteLine(label1.Text, "\n");


        }

       

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            float deger;
            deger = trackBar2.Value;
            deger = deger / 10;
            label2.Text = deger.ToString();
   
            //     using (sw = System.IO.File.AppendText(loc))
            //        sw.WriteLine(label2.Text, "\n");

        }

        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            float deger;
            deger = trackBar3.Value;
            deger = deger / 10;
            label3.Text = deger.ToString();

            //    using (sw = System.IO.File.AppendText(loc))
            //        sw.WriteLine(label3.Text, "\n");
        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {

            label4.Text = trackBar4.Value.ToString();

            //   using (sw = System.IO.File.AppendText(loc))
            //       sw.WriteLine(label4.Text, "\n");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (System.IO.File.Exists(loc))
            {
                System.IO.File.Delete(loc);
            }
            using (sw = System.IO.File.AppendText(loc))
            {
                sw.WriteLine(label1.Text, "\n");
                sw.WriteLine(label2.Text, "\n");
                sw.WriteLine(label3.Text, "\n");
                sw.WriteLine(label4.Text);
            }


            /*       text1 = System.IO.File.ReadAllText(loc);
                   text1 = string.Format(text1.Replace(System.IO.File.ReadLines(loc).First(), label1.Text.ToString()));
                   System.IO.File.WriteAllText(loc, text1);

                   text2 = System.IO.File.ReadAllText(loc);
                   text2 = string.Format(text2.Replace(System.IO.File.ReadLines(loc).ElementAt(1), label2.Text.ToString()));
                   System.IO.File.WriteAllText(loc, text2);

                   text3 = System.IO.File.ReadAllText(loc);
                   text3 = string.Format(text3.Replace(System.IO.File.ReadLines(loc).ElementAt(2), label3.Text.ToString()));
                   System.IO.File.WriteAllText(loc, text3);

                   text4 = System.IO.File.ReadAllText(loc);
                   text4 = string.Format(text4.Replace(System.IO.File.ReadLines(loc).ElementAt(3), label4.Text.ToString()));
                   System.IO.File.WriteAllText(loc, text4);
       */
            //        using (sw = System.IO.File.AppendText(loc))
            //         sw.WriteLine(label1.Text, "\n");
            //      string line1 = System.IO.File.ReadLines(loc).First();

        }
/*
        private void button5_Click(object sender, EventArgs e)
        {
            string text = System.IO.File.ReadAllText(loc);
            text = text.Replace(System.IO.File.ReadLines(loc).ElementAt(1), label2.Text);
            System.IO.File.WriteAllText(loc, text);

      //      using (sw = System.IO.File.AppendText(loc))
       //         sw.WriteLine(label2.Text, "\n");
            
        }

        private void button6_Click(object sender, EventArgs e)
        {
            string text = System.IO.File.ReadAllText(loc);
            text = text.Replace(System.IO.File.ReadLines(loc).ElementAt(2), label3.Text);
            System.IO.File.WriteAllText(loc, text);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            string text = System.IO.File.ReadAllText(loc);
            text = text.Replace(System.IO.File.ReadLines(loc).ElementAt(3), label4.Text);
            System.IO.File.WriteAllText(loc, text);
        }
        */
    }
}

