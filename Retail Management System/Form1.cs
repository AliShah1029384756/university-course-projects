using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Project
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Start();

        }
        int Startpoint = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            Startpoint += 5;
            progressBar1.Value = Startpoint;

            if (progressBar1.Value == 100)
            {
                progressBar1.Value = 0;
                timer1.Stop();
                Login log = new Login();
                this.Hide();
                log.Show();
            }
        }

        private void progressBar1_Click(object sender, EventArgs e)
        {

        }

        private void progressBar1_ForeColorChanged(object sender, EventArgs e)
        {
        }

        private void progressBar1_BackColorChanged(object sender, EventArgs e)
        {


        }


    }


}
