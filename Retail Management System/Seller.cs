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
    public partial class Seller : Form
    {
        OracleConnection con = new OracleConnection(@"DATA SOURCE = localhost:1521/XE; USER ID=shayan;PASSWORD=system;");

        public Seller()
        {
            InitializeComponent();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void populate()
        {
            try
            {
                if (con.State != ConnectionState.Open)  // Check if the connection is already open
                {
                    con.Open();
                }
                string query = "select * from seller";
                OracleDataAdapter sda = new OracleDataAdapter(query, con);
                OracleCommandBuilder builder = new OracleCommandBuilder(sda);

                var ds = new DataSet();
                sda.Fill(ds);
                sellerdgv.DataSource = ds.Tables[0];
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                if (con.State == ConnectionState.Open)  // Close the connection
                {
                    con.Close();
                }
            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            try
            {
                if (con.State != ConnectionState.Open)
                {
                    con.Open();
                }
                string query = "INSERT INTO Seller (ID, Name, Age, Mobileno, Password) VALUES (:ID, :Name, :Age, :Mobileno, :Password)";
                OracleCommand cmd = new OracleCommand(query, con);
                cmd.Parameters.Add(new OracleParameter("ID", txtID.Text));
                cmd.Parameters.Add(new OracleParameter("Name", txtName.Text));
                cmd.Parameters.Add(new OracleParameter("Age", agetxt.Text));
                cmd.Parameters.Add(new OracleParameter("Mobileno", mobilenotxt.Text));
                cmd.Parameters.Add(new OracleParameter("Password", passwordtxt.Text));

                cmd.ExecuteNonQuery();
                MessageBox.Show("Seller Added Successfully");
                populate();  // Ensure this refreshes the data grid after adding a new seller.

                // Clear textboxes
                txtID.Text = "";
                txtName.Text = "";
                agetxt.Text = "";
                mobilenotxt.Text = "";
                passwordtxt.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                if (con.State == ConnectionState.Open)
                {
                    con.Close();
                }
            }
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                if (string.IsNullOrEmpty(txtID.Text) ||
                    string.IsNullOrEmpty(txtName.Text) ||
                    string.IsNullOrEmpty(agetxt.Text) ||
                    string.IsNullOrEmpty(mobilenotxt.Text) ||
                    string.IsNullOrEmpty(passwordtxt.Text))
                {
                    MessageBox.Show("Missing Information");
                }
                else
                {
                    if (con.State != ConnectionState.Open)
                    {
                        con.Open();
                    }

                    string query = "UPDATE Seller SET Name = :Name, Age = :Age, MobileNo = :MobileNo, Password = :Password WHERE ID = :ID";

                    using (OracleCommand cmd = new OracleCommand(query, con))
                    {
                        cmd.Parameters.Add(new OracleParameter("SellerName", txtName.Text));
                        cmd.Parameters.Add(new OracleParameter("SellerAge", agetxt.Text));
                        cmd.Parameters.Add(new OracleParameter("SellerMobileNo", mobilenotxt.Text));
                        cmd.Parameters.Add(new OracleParameter("SellerPassword", passwordtxt.Text));
                        cmd.Parameters.Add(new OracleParameter("SellerID", txtID.Text));

                        cmd.ExecuteNonQuery();
                    }

                    MessageBox.Show("Seller Successfully Updated");
                    populate();

                    // Clear textboxes
                    txtID.Text = "";
                    txtName.Text = "";
                    agetxt.Text = "";
                    mobilenotxt.Text = "";
                    passwordtxt.Text = "";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                if (con.State == ConnectionState.Open)
                {
                    con.Close();
                }
            }
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            try
            {
                if (txtID.Text == "")
                {
                    MessageBox.Show("Select the seller to Delete");
                }
                else
                {
                    if (con.State != ConnectionState.Open)
                    {
                        con.Open();
                    }

                    OracleCommand cmd = new OracleCommand("DELETE FROM seller WHERE Id = :Id", con);
                    cmd.Parameters.Add("SellerId", OracleDbType.Int32).Value = int.Parse(txtID.Text);
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("Seller Deleted Successfully");

                    // Clear textboxes
                    txtID.Text = "";
                    txtName.Text = "";
                    agetxt.Text = "";
                    mobilenotxt.Text = "";
                    passwordtxt.Text = "";
                    populate();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                if (con.State == ConnectionState.Open)
                {
                    con.Close();
                }
            }
        }

        private void Espurt_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void Seller_Load(object sender, EventArgs e)
        {
            populate();
        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            this.Hide();
            Login login = new Login();
            login.Show();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void productbtn_Click(object sender, EventArgs e)
        {
            Product_Form prod = new Product_Form();
            prod.Show();
            this.Hide();
        }

        private void btnProducts_Click(object sender, EventArgs e)
        {
            Category cat = new Category();
            cat.Show();
            this.Hide();
        }

        private void btnSelling_Click(object sender, EventArgs e)
        {
            Selling sel = new Selling();
            sel.Show();
            this.Hide();
        }
    }
}
