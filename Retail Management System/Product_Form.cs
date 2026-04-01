using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Oracle.ManagedDataAccess.Client;

namespace Project
{
    public partial class Product_Form : Form
    {
        OracleConnection con = new OracleConnection(@"DATA SOURCE = localhost:1521/XE; USER ID=shayan;PASSWORD=system;");

        public Product_Form()
        {
            InitializeComponent();
        }

        private void Product_Form_Load(object sender, EventArgs e)
        {
            FillCategory();
            populate();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit(); 
        }

        private void productdgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            txtID.Text = productdgv.SelectedRows[0].Cells[0].Value.ToString();
            nametext.Text = productdgv.SelectedRows[0].Cells[1].Value.ToString();
            quantitytxt.Text = productdgv.SelectedRows[0].Cells[2].Value.ToString();
            pricetxt.Text = productdgv.SelectedRows[0].Cells[3].Value.ToString();
            selectcategory.SelectedValue = productdgv.SelectedRows[0].Cells[4].Value.ToString();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            try
            {
                con.Open();
                string query = "INSERT INTO product (id, name, quantity, price, category) VALUES (:id, :name, :quantity, :price, :category)";
                OracleCommand cmd = new OracleCommand(query, con);
                cmd.Parameters.Add(new OracleParameter("id", int.Parse(txtID.Text)));
                cmd.Parameters.Add(new OracleParameter("name", nametext.Text));
                cmd.Parameters.Add(new OracleParameter("quantity", int.Parse(quantitytxt.Text)));
                cmd.Parameters.Add(new OracleParameter("price", pricetxt.Text)); // Handle as string since price is VARCHAR
                cmd.Parameters.Add(new OracleParameter("category", selectcategory.SelectedValue.ToString()));
                cmd.ExecuteNonQuery();
                MessageBox.Show("Product Added Successfully");
                con.Close();
                populate();
                txtID.Text = "";
                nametext.Text = "";
                quantitytxt.Text = "";
                pricetxt.Text = "";
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }



        private void FillCategory()
        {
            con.Open();
            OracleCommand cmd = new OracleCommand("Select name from category", con);
            OracleDataReader rdr;
            rdr = cmd.ExecuteReader();
            DataTable dt = new DataTable();
            dt.Columns.Add("Name", typeof(string));
            dt.Load(rdr);
            selectcategory.ValueMember = "Name";
            selectcategory.DataSource = dt;
            selectcategory.ValueMember = "Name";
            selectcategory.DataSource = dt;
            con.Close();
        }

        private void populate()
        {
            con.Open();
            string query = "Select * from product";
            OracleDataAdapter sda = new OracleDataAdapter(query, con);
            OracleCommandBuilder builder = new OracleCommandBuilder(sda);
            var ds = new DataSet();
            sda.Fill(ds);
            productdgv.DataSource = ds.Tables[0];
            con.Close();
        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            try
            {
                if (string.IsNullOrEmpty(txtID.Text) || string.IsNullOrEmpty(nametext.Text) ||
                    string.IsNullOrEmpty(quantitytxt.Text) || string.IsNullOrEmpty(pricetxt.Text))
                {
                    MessageBox.Show("Missing Information");
                    return;
                }

                con.Open();
                string query = "UPDATE product SET name = :name, quantity = :quantity, price = :price, category = :category WHERE id = :id";
                OracleCommand cmd = new OracleCommand(query, con);
                cmd.Parameters.Add(new OracleParameter("name", nametext.Text));
                cmd.Parameters.Add(new OracleParameter("quantity", int.Parse(quantitytxt.Text)));
                cmd.Parameters.Add(new OracleParameter("price", pricetxt.Text));
                cmd.Parameters.Add(new OracleParameter("category", selectcategory.SelectedValue.ToString()));
                cmd.Parameters.Add(new OracleParameter("id", int.Parse(txtID.Text)));
                cmd.ExecuteNonQuery();
                MessageBox.Show("Product Successfully Updated");
                con.Close();
                populate();
                txtID.Text = "";
                nametext.Text = "";
                quantitytxt.Text = "";
                pricetxt.Text = "";
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
                if (txtID.Text == "")
                {
                    MessageBox.Show("Select the Product to Delete");
                }
                else
                {
                    con.Open();
                    string query = "DELETE FROM Product WHERE ID = :ID";
                    OracleCommand cmd = new OracleCommand(query, con);
                    cmd.Parameters.Add(new OracleParameter("ID", int.Parse(txtID.Text)));
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("Product deleted successfully");
                    con.Close();
                    populate();
                    txtID.Text = "";
                    nametext.Text = "";
                    quantitytxt.Text = "";
                    pricetxt.Text = "";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

        }

        private void comboBox1_SelectionChangeCommitted(object sender, EventArgs e)
        {

        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            this.Hide();
            Login login = new Login();
            login.Show();
        }

        private void selectcategory_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void refreshbtn_Click(object sender, EventArgs e)
        {
            populate();
        }

        private void btnProducts_Click(object sender, EventArgs e)
        {
            Category cat = new Category();
            cat.Show();
            this.Hide();
        }

        private void btnManager_Click(object sender, EventArgs e)
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

        private void searchcategory_SelectedValueChanged(object sender, EventArgs e)
        {

        }

        private void searchcategory_SelectionChangeCommitted(object sender, EventArgs e)
        {
            try
            {
                con.Open();
                string query = "SELECT * FROM Product WHERE Category = :Category";
                OracleCommand cmd = new OracleCommand(query, con);
                cmd.Parameters.Add(new OracleParameter("Category", selectcategory.SelectedValue.ToString()));

                OracleDataAdapter sda = new OracleDataAdapter(cmd);
                DataSet ds = new DataSet();
                sda.Fill(ds);
                productdgv.DataSource = ds.Tables[0];
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }
            finally
            {
                con.Close();
            }
        }
    }
}
