using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;

namespace InterfaceProject
{
    public partial class Form1 : Form
    {
        System.Windows.Forms.Timer aTimer;
        System.Diagnostics.Process process = new System.Diagnostics.Process();
        string line;
        string path;
        string loc = System.IO.Path.GetDirectoryName(Application.ExecutablePath) + "/config.txt";
        System.IO.StreamWriter sw;

        public Form1()
        {
            InitializeComponent();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            checkBox1.Enabled = false;
            checkBox2.Enabled = false;
            checkBox3.Enabled = false;
            checkBox4.Enabled = false;
            checkBox5.Enabled = false;
            if (textBox1.Text == "" && checkBox1.Checked == false)
            {
                MessageBox.Show("Dosya ismi boş bırakılamaz!", "Hata",
    MessageBoxButtons.OK, MessageBoxIcon.Error);
                checkBox1.Enabled = true;
                checkBox2.Enabled = true;
                checkBox3.Enabled = true;
                checkBox4.Enabled = true;
                checkBox5.Enabled = true;
                return;
            }


            string fLoc = "";
            fLoc = System.IO.Path.GetDirectoryName(Application.ExecutablePath);
            fLoc = fLoc + "/EVM.exe";
            fLoc.Replace("/", "//");



            System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();
            startInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            startInfo.FileName = "CMD.exe";
            startInfo.Arguments = "/C" + fLoc;
            process.StartInfo = startInfo;

            process.StartInfo.UseShellExecute = false;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.CreateNoWindow = true;

            process.Start();


            aTimer = new System.Windows.Forms.Timer();
            aTimer.Interval = 10;
            aTimer.Tick += new EventHandler(OnTimedEvent);
            aTimer.Enabled = true;
            aTimer.Start();
        }
       

        private void OnTimedEvent(Object source, EventArgs e)
        {
            
        
            line = process.StandardOutput.ReadLine();  
  
            if(line != null)
            {
                label6.Text = line + "\n BPM";
            }
            


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
                aTimer.Stop();
            }
            checkBox1.Enabled = true;
            checkBox2.Enabled = true;
            checkBox3.Enabled = true;
            checkBox4.Enabled = true;
            checkBox5.Enabled = true;
           
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string oSelectedFile = "";
            System.Windows.Forms.OpenFileDialog oDlg = new System.Windows.Forms.OpenFileDialog();
            if (System.Windows.Forms.DialogResult.OK == oDlg.ShowDialog())
            {
                oSelectedFile = oDlg.FileName;
                path = System.IO.Path.GetFileName(oDlg.FileName);
                textBox1.Text = path;

            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked == true)
            {
                textBox1.Enabled = false;
                button3.Enabled = false;
            }
            else
            {
                textBox1.Enabled = true;
                button3.Enabled = true;
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {

            float deger;
            deger = trackBar1.Value;
            deger = deger / 10;
            label1.Text = deger.ToString().Replace(",", ".");

        }



        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            float deger;
            deger = trackBar2.Value;
            deger = deger / 10;
            label2.Text = deger.ToString().Replace(",", ".");

        }

        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            float deger;
            deger = trackBar3.Value;
            deger = deger / 10;
            label3.Text = deger.ToString().Replace(",", ".");

        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {

            label4.Text = trackBar4.Value.ToString().Replace(",", ".");

        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "" && checkBox1.Checked == false)
            {
                MessageBox.Show("Dosya ismi boş bırakılamaz!", "Hata",
    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            if (System.IO.File.Exists(loc))
            {
                System.IO.File.Delete(loc);
            }
            using (sw = System.IO.File.AppendText(loc))
            {

                if (checkBox1.Checked == false)
                {
                    sw.WriteLine(textBox1.Text, "\n");
                }
                else
                {
                    sw.WriteLine("kamera", "\n");
                }


                sw.WriteLine(label1.Text, "\n");
                sw.WriteLine(label2.Text, "\n");
                sw.WriteLine(label3.Text, "\n");
                sw.WriteLine(label4.Text);
                if (checkBox2.Checked == false)
                {
                    sw.WriteLine("false", "\n");
                }
                else
                {
                    sw.WriteLine("true", "\n");
                }
                if (checkBox3.Checked == true)
                {
                    sw.WriteLine("2", "\n");
                }
                if (checkBox4.Checked == true)
                {
                    sw.WriteLine("1", "\n");
                }
                if (checkBox5.Checked == true)
                {
                    sw.WriteLine("0", "\n");
                }
                if (checkBox3.Checked == false && checkBox4.Checked == false && checkBox5.Checked == false)
                {
                    sw.WriteLine("-1", "\n");
                }

            }



        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked == true)
            {
                checkBox4.Enabled = false;
                checkBox5.Enabled = false;
            }
            else
            {
                checkBox4.Enabled = true;
                checkBox5.Enabled = true;
            }
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked == true)
            {
                checkBox3.Enabled = false;
                checkBox5.Enabled = false;
            }
            else
            {
                checkBox3.Enabled = true;
                checkBox5.Enabled = true;
            }
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox5.Checked == true)
            {
                checkBox4.Enabled = false;
                checkBox3.Enabled = false;
            }
            else
            {
                checkBox4.Enabled = true;
                checkBox3.Enabled = true;
            }
        }
    }
}
