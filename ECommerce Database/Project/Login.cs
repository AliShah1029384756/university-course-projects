using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Oracle.ManagedDataAccess.Client;

namespace Project
{
    public partial class Login : Form
    {
        public static string Sellername = "";
        OracleConnection con = new OracleConnection(@"DATA SOURCE = localhost:1521/XE; USER ID=shayan;PASSWORD=system;");

        public Login()
        {
            InitializeComponent();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void selectRole_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Login_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            if (txtUsername.Text == "" || txtPassword.Text == "")
            {
                MessageBox.Show("Please Enter the Username and Password");
            }
            else if (cbSelectRole.SelectedIndex > -1)
            {
                if (cbSelectRole.SelectedItem.ToString() == "Admin")
                {
                    if (txtUsername.Text == "Admin" && txtPassword.Text == "Admin")
                    {
                        Product_Form Prod = new Product_Form();
                        Prod.Show();
                        this.Hide();
                    }
                    else
                    {
                        MessageBox.Show("If You are Admin, Enter the Correct Username and Password");
                    }
                }
                else
                {
                    // Assuming you are handling Seller login
                    con.Open();
                    OracleDataAdapter sda = new OracleDataAdapter("SELECT count(*) FROM Seller WHERE Name = :name", con);
                    sda.SelectCommand.Parameters.Add(new OracleParameter(":username", txtUsername.Text));
                    DataTable dt = new DataTable();
                    sda.Fill(dt);

                    if (dt.Rows[0][0].ToString() == "1")
                    {
                        Sellername = txtUsername.Text;
                        Selling sell = new Selling();
                        sell.Show();
                        this.Hide();
                    }
                    else
                    {
                        MessageBox.Show("Wrong Username and Password");
                    }

                    con.Close();
                }
            }
            else
            {
                MessageBox.Show("Select the Role to Login");
            }

        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            txtUsername.Text = "";
            txtPassword.Text = "";
        }
    }
}
