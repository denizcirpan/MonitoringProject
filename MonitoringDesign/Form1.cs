using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

//test
namespace MonitoringDesign
{
    public partial class Form1 : Form
    {
        
        string filePath = "C:\\Users\\User\\source\\repos\\Monitoring\\Monitoring\\output.txt";
       
        public Form1()
        {
            
            InitializeComponent();
            

        }
        private void TimerCallback(object state)
        {

            // This method will be called after 30 seconds
            // Do your work here
            // ...
            // Read the entire file into a string
            string fileContent = File.ReadAllText(filePath);
            string[] variables = fileContent.Split(',');
          

            // If you need to update UI elements from this method, use BeginInvoke
            BeginInvoke(new Action(() =>
            {

                // Display the separated values
                textBox1.Text = variables[0];
                textBox2.Text = variables[1];
                textBox3.Text = variables[2];
                Console.WriteLine("Timer");
                

            }));

            System.Threading.Timer timer = new System.Threading.Timer(TimerCallback, null, 5000, Timeout.Infinite);
        }
       

        private void Form1_Load(object sender, EventArgs e)
        {

            System.Threading.Timer timer = new System.Threading.Timer(TimerCallback, null, 5000, Timeout.Infinite);

            try
            {
                // Read the entire file into a string
                string fileContent = File.ReadAllText(filePath);
                string[] variables = fileContent.Split(',');


                // Display the separated values
                textBox1.Text = variables[0];
                textBox2.Text = variables[1];
                textBox3.Text = variables[2];

                // Display the file content
                //Console.WriteLine("File Content:");
                Console.WriteLine(fileContent);
               

            }
            catch (FileNotFoundException)
            {
                Console.WriteLine("File not found.");
            }
          
        }
       





    }

    }

