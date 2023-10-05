using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;


namespace MonitoringDesign
{
    public partial class Form1 : Form
    {
        //verilerin okunması için verilen dosya yolu 
        string filePath = "../../../Monitoring/output.txt";  
        public Form1()
        {
            InitializeComponent();
        }

        //30 saniye dolunca forma verilerin tekrar yazılması için çağrılan fonksiyon
        private void TimerCallback(object state)
        {
            string fileContent = File.ReadAllText(filePath);
            string[] variables = fileContent.Split(',');
          
            BeginInvoke(new Action(() =>
            {
                //Dosyadan okunan verilen class içerisinde bulunan nesnelere atanması
                ClassCPU cpuInstance = new ClassCPU(variables[0]);
                ClassRAM ramInstance = new ClassRAM(variables[1], variables[2], variables[3]);
                ClassHDD hddInstance = new ClassHDD(variables[4], variables[5]);

                //Atanan değerlerin textbox'lar içerisine yazılması 

                textBox1.Text = cpuInstance.cpuusage;
                textBox2.Text = "0";
                textBox3.Text = ramInstance.ramusage;
                textBox4.Text = hddInstance.hddempty;

            }));

            System.Threading.Timer timer = new System.Threading.Timer(TimerCallback, null, 30000, Timeout.Infinite);
        }
        private void Form1_Load(object sender, EventArgs e)
        {

            System.Threading.Timer timer = new System.Threading.Timer(TimerCallback, null, 30000, Timeout.Infinite);

            try
            {
               
                string fileContent = File.ReadAllText(filePath);
                string[] variables = fileContent.Split(',');

                ClassCPU cpuInstance = new ClassCPU(variables[0]);
                ClassRAM ramInstance = new ClassRAM(variables[1], variables[2], variables[3]);
                ClassHDD hddInstance = new ClassHDD(variables[4], variables[5]);


                label1.Visible = true;
                label2.Visible = true;
                label3.Visible = true;
                label4.Visible = true;
                label5.Visible = false;
                label6.Visible = false;
                label7.Visible = false;

                textBox1.Visible = true;
                textBox2.Visible = true;
                textBox3.Visible = true;
                textBox4.Visible = true;
                textBox5.Visible = false;
                textBox6.Visible = false;
                textBox7.Visible = false;

                label1.Text = "CPU USAGE";
                label2.Text = "CPU TEMPERATURE";
                label3.Text = "RAM USAGE";
                label4.Text = "HDD FREE SPACE";

                textBox1.Text = cpuInstance.cpuusage;
                textBox2.Text = "0";
                textBox3.Text = ramInstance.ramusage;
                textBox4.Text = hddInstance.hddempty;

            }
            catch (FileNotFoundException)
            {
                Console.WriteLine("File not found.");
            }
          
        }

        //home button fonksiyon
        private void homebutton_Click(object sender, EventArgs e)
        {
            string fileContent = File.ReadAllText(filePath);
            string[] variables = fileContent.Split(',');


            ClassCPU cpuInstance = new ClassCPU(variables[0]);
            ClassRAM ramInstance = new ClassRAM(variables[1], variables[2], variables[3]);
            ClassHDD hddInstance = new ClassHDD(variables[4], variables[5]);

            label1.Visible = true;
            label2.Visible = true;
            label3.Visible = true;
            label4.Visible = true;
            label5.Visible = false;
            label6.Visible = false;
            label7.Visible = false;

            textBox1.Visible = true;
            textBox2.Visible = true;
            textBox3.Visible = true;
            textBox4.Visible = true;
            textBox5.Visible = false;
            textBox6.Visible = false;
            textBox7.Visible = false;

            label1.Text = "CPU USAGE";
            label2.Text = "CPU TEMPERATURE";
            label3.Text = "RAM USAGE";
            label4.Text = "HDD FREE SPACE";

            textBox1.Text = cpuInstance.cpuusage;
            textBox3.Text = ramInstance.ramusage;
            textBox2.Text = "0";
            textBox4.Text = hddInstance.hddempty;

        }
        //cpu button fonksiyon
        private void cpubutton_Click(object sender, EventArgs e)
        {
            string fileContent = File.ReadAllText(filePath);
            string[] variables = fileContent.Split(',');

            ClassCPU cpuInstance = new ClassCPU(variables[0]);

            label1.Visible = false;
            label2.Visible = false;
            label3.Visible = false;
            label4.Visible = false;
            label5.Visible = true;
            label6.Visible = true;
            label7.Visible = false;

            textBox1.Visible = false;
            textBox2.Visible = false;
            textBox3.Visible = false;
            textBox4.Visible = false;
            textBox5.Visible = true;
            textBox6.Visible = true;
            textBox7.Visible = false;

            label5.Text = "CPU USAGE";
            label6.Text = "CPU TEMPERATURE";

            textBox5.Text = cpuInstance.cpuusage;
            textBox6.Text = "0";

        }
        //ram button fonksiyon
        private void rambutton_Click(object sender, EventArgs e)
        {
            string fileContent = File.ReadAllText(filePath);
            string[] variables = fileContent.Split(',');


            ClassRAM ramInstance = new ClassRAM(variables[1], variables[2], variables[3]);

            label1.Visible = true;
            label2.Visible = true;
            label3.Visible = false;
            label4.Visible = false;
            label5.Visible = false;
            label6.Visible = false;
            label7.Visible = true;

            textBox1.Visible = true;
            textBox2.Visible = true;
            textBox3.Visible = false;
            textBox4.Visible = false;
            textBox5.Visible = false;
            textBox6.Visible = false;
            textBox7.Visible = true;

            label1.Text = "RAM USAGE";
            label2.Text = "RAM FULL";
            label7.Text = "RAM EMPTY";

            textBox1.Text = ramInstance.ramusage;
            textBox2.Text = ramInstance.ramtotal;
            textBox7.Text = ramInstance.ramempty;

        }

        //disk button fonksiyonu
        private void diskbutton_Click(object sender, EventArgs e)
        {
            string fileContent = File.ReadAllText(filePath);
            string[] variables = fileContent.Split(',');

            ClassHDD hddInstance = new ClassHDD(variables[4], variables[5]);

            label1.Visible = false;
            label2.Visible = false;
            label3.Visible = false;
            label4.Visible = false;
            label5.Visible = true;
            label6.Visible = true;
            label7.Visible = false;

            textBox1.Visible = false;
            textBox2.Visible = false;
            textBox3.Visible = false;
            textBox4.Visible = false;
            textBox5.Visible = true;
            textBox6.Visible = true;
            textBox7.Visible = false;

            label5.Text = "HDD FREE SPACE";
            label6.Text = "HDD TOTAL SPACE";

            textBox5.Text = hddInstance.hddempty;
            textBox6.Text = hddInstance.hddfull;

        }
    }

    //Donanımların bilgilerinin düzenli yazılması için oluşturulan classlar
    public class ClassCPU
    {
        public string cpuusage { get; set; }
        //public string cputemperature { get; set; }
        public ClassCPU(string cpuusage)
        {
            this.cpuusage = cpuusage;
            //this.cputempereature= cputempereature;
        }
    }
    public class ClassRAM
    {
        public string ramusage { get; set; }
        public string ramtotal { get; set; }
        public string ramempty { get; set; }

        public ClassRAM(string ramusage, string ramtotal, string ramempty)
        {
            this.ramusage = ramusage;
            this.ramtotal = ramtotal;
            this.ramempty = ramempty;
        }
    }
    public class ClassHDD
    {
        public string hddfull { get; set; }
        public string hddempty { get; set; }
        public ClassHDD(string hddfull, string hddempty)
        {
            this.hddfull = hddfull;
            this.hddempty = hddempty;
        }
    }
}

