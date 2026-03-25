using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Oracle.ManagedDataAccess.Client;

namespace Project
{
    public partial class Category : Form
    {
        public Category()
        {
            InitializeComponent();
        }

        OracleConnection con = new OracleConnection(@"DATA SOURCE = localhost:1521/XE; USER ID=shayan;PASSWORD=system;");

        private void maskedTextBox3_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }

        private void maskedTextBox1_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void maskedTextBox2_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void populate()
        {
            con.Open();
            string query = "select * from category";
            OracleDataAdapter sda = new OracleDataAdapter(query, con);
            OracleCommandBuilder builder = new OracleCommandBuilder(sda);
            var ds = new DataSet();
            sda.Fill(ds);
            categorydgv.DataSource = ds.Tables[0];
            con.Close();

        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            try
            {
                con.Open();
                string query = "INSERT INTO Category (ID, Name, Description) VALUES (:ID, :Name, :Description)";
                OracleCommand cmd = new OracleCommand(query, con);
                cmd.Parameters.Add(new OracleParameter("ID", txtID.Text));
                cmd.Parameters.Add(new OracleParameter("Name", txtName.Text));
                cmd.Parameters.Add(new OracleParameter("Description", txtDescription.Text));
                cmd.ExecuteNonQuery();
                MessageBox.Show("Category Added Successfully");
                con.Close();
                populate();
                txtID.Text = "";
                txtName.Text = "";
                txtDescription.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void categorydgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            txtID.Text = categorydgv.SelectedRows[0].Cells[0].Value.ToString();
            txtName.Text = categorydgv.SelectedRows[0].Cells[1].Value.ToString();
            txtDescription.Text = categorydgv.SelectedRows[0].Cells[2].Value.ToString();
        }

        private void Category_Load(object sender, EventArgs e)
        {
            populate();

        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                // Check for missing information
                if (string.IsNullOrWhiteSpace(txtID.Text) ||
                    string.IsNullOrWhiteSpace(txtName.Text) ||
                    string.IsNullOrWhiteSpace(txtDescription.Text))
                {
                    MessageBox.Show("Missing Information");
                }
                else
                {
                    // Open the connection
                    con.Open();

                    // Define the update query with parameters
                    string query = "UPDATE Category " +
                                   "SET Name = :Name, Description = :Description " +
                                   "WHERE Id = :Id";

                    // Create the OracleCommand
                    OracleCommand cmd = new OracleCommand(query, con);

                    // Add parameters
                    cmd.Parameters.Add(new OracleParameter("Name", txtName.Text));
                    cmd.Parameters.Add(new OracleParameter("Description", txtDescription.Text));
                    cmd.Parameters.Add(new OracleParameter("Id", txtID.Text));

                    cmd.ExecuteNonQuery();
                    MessageBox.Show("Category has been updated successfully");
                    con.Close();
                    populate();
                    txtID.Text = "";
                    txtName.Text = "";
                    txtDescription.Text = "";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            try
            {
                if (string.IsNullOrWhiteSpace(txtID.Text))
                {
                    MessageBox.Show("Select a Category ID to delete");
                }
                else
                {
                    con.Open();
                    string query = "DELETE FROM Category WHERE Id = :Id";
                    OracleCommand cmd = new OracleCommand(query, con);
                    cmd.Parameters.Add(new OracleParameter("Id", txtID.Text));
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("Category has been deleted successfully");
                    con.Close();
                    populate();
                    txtID.Text = "";
                    txtName.Text = "";
                    txtDescription.Text = "";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            this.Hide();
            Login login = new Login();
            login.Show();
        }

        private void btnProducts_Click(object sender, EventArgs e)
        {
            Product_Form prod = new Product_Form();
            prod.Show();
            this.Hide();
        }

        private void btnSeller_Click(object sender, EventArgs e)
        {
            Seller sell = new Seller();
            sell.Show();
            this.Hide();
        }

        private void btnSelling_Click(object sender, EventArgs e)
        {
            Selling sell = new Selling();
            sell.Show();       
            this.Hide();    
        }
    }
}

