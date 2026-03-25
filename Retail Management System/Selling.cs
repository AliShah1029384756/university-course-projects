using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;
using Oracle.ManagedDataAccess.Client;

namespace Project
{
    public partial class Selling : Form
    {
        OracleConnection con = new OracleConnection(@"DATA SOURCE = localhost:1521/XE; USER ID=shayan;PASSWORD=system;");

        public Selling()
        {
            InitializeComponent();
        }

        private void populate()
        {
            con.Open();
            string query = "SELECT Name, Quantity FROM Product";
            OracleDataAdapter oda = new OracleDataAdapter(query, con);
            OracleCommandBuilder builder = new OracleCommandBuilder(oda);
            var ds = new DataSet();
            oda.Fill(ds);
            OrderDGV.DataSource = ds.Tables[0];
            con.Close();
        }

        private void PopulateBills()
        {
            try
            {
                con.Open();
                string query = "SELECT * FROM Bill";
                using (OracleDataAdapter oda = new OracleDataAdapter(query, con))
                {
                    var ds = new DataSet();
                    oda.Fill(ds);
                    Billdgv.DataSource = ds.Tables[0];
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }
            finally
            {
                con.Close(); // Ensure connection is closed.
            }
        }

        private void productbtn_Click(object sender, EventArgs e)
        {
            Seller sell = new Seller();
            sell.Show();
            this.Hide();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            this.Hide();
            Login login = new Login();
            login.Show();
        }

        private void btnProduct_Click(object sender, EventArgs e)
        {
            Product_Form prod = new Product_Form();
            prod.Show();
            this.Hide();
        }

        private void btnCategory_Click(object sender, EventArgs e)
        {
            Category cat = new Category();
            cat.Show();
            this.Hide();
        }

        private void AddProductbtn_Click(object sender, EventArgs e)
        {
            if (!int.TryParse(pricetxt.Text, out int price) || !int.TryParse(txtQuantity.Text, out int quantity))
            {
                MessageBox.Show("Please enter valid numeric values for price and quantity.");
                return;
            }

            if (quantity < 0 || price < 0)
            {
                MessageBox.Show("Quantity and price cannot be negative.");
                return;
            }

            int total = price * quantity;

            DataGridViewRow newRow = new DataGridViewRow();
            newRow.CreateCells(OrderDGV);
            newRow.Cells["ID"].Value = n + 1;
            newRow.Cells["ProductNameColumn"].Value = txtName.Text;
            newRow.Cells["QuantityColumn"].Value = quantity;
            newRow.Cells["PriceColumn"].Value = price;
            newRow.Cells["TotalColumn"].Value = total;

            OrderDGV.Rows.Add(newRow);

            n++;
            Grdtotal += total;

            lblAmount.Text = Grdtotal.ToString();


        }

        private void FillCategory()
        {
       con.Open();
    OracleCommand cmd = new OracleCommand("SELECT Name FROM Category", con);
        OracleDataReader rdr = cmd.ExecuteReader();
        DataTable dt = new DataTable();
        dt.Columns.Add("Name", typeof(string));
        dt.Load(rdr);
        selectcategory.ValueMember = "Name";
        selectcategory.DataSource = dt;
        con.Close();
        }

        private void Selling_Load(object sender, EventArgs e)
        {
            populate();
            PopulateBills();
            FillCategory();
            lblsellername.Text = Login.Sellername;

        }

        private void productdgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            txtName.Text = OrderDGV.SelectedRows[0].Cells[0].Value.ToString();
            txtQuantity.Text = OrderDGV.SelectedRows[0].Cells[1].Value.ToString();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            lblDate.Text = DateTime.Today.ToString("dd/MM/yyyy");
        }
        int Grdtotal = 0, n = 0;

        private void PrintDocument_PrintPage(object sender, System.Drawing.Printing.PrintPageEventArgs e)
        {
            try
            {
                e.Graphics.DrawString("ESpurt", new Font("Century Gothic", 25, FontStyle.Bold), Brushes.Red, new Point(230, 20));

                // Ensure the selected rows are not null or empty before accessing
                if (Billdgv.SelectedRows.Count > 0)
                {
                    e.Graphics.DrawString("ID: " + Billdgv.SelectedRows[0].Cells[0].Value.ToString(), new Font("Century Gothic", 20, FontStyle.Bold), Brushes.Blue, new Point(100, 70));
                    e.Graphics.DrawString("Name: " + Billdgv.SelectedRows[0].Cells[1].Value.ToString(), new Font("Century Gothic", 20, FontStyle.Bold), Brushes.Blue, new Point(100, 100));
                    e.Graphics.DrawString("Date: " + Billdgv.SelectedRows[0].Cells[2].Value.ToString(), new Font("Century Gothic", 20, FontStyle.Bold), Brushes.Blue, new Point(100, 130));
                    e.Graphics.DrawString("TotalAmount: " + Billdgv.SelectedRows[0].Cells[3].Value.ToString(), new Font("Century Gothic", 20, FontStyle.Bold), Brushes.Blue, new Point(100, 160));
                }

                e.Graphics.DrawString("ESpurt", new Font("Century Gothic", 25, FontStyle.Bold), Brushes.Red, new Point(230, 230));
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error printing the document: " + ex.Message);
            }
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void refreshbtn_Click(object sender, EventArgs e)
        {
            populate();
        }

        private void selectcategory_SelectionChangeCommitted(object sender, EventArgs e)
        {
            con.Open();

            try
            {
                string query = "SELECT Name, Quantity FROM Product WHERE Category = :Category";
                using (OracleDataAdapter sda = new OracleDataAdapter(query, con))
                {
                    sda.SelectCommand.Parameters.Add(new OracleParameter(":Category", selectcategory.SelectedValue.ToString()));
                    var ds = new DataSet();
                    sda.Fill(ds);
                    OrderDGV.DataSource = ds.Tables[0];
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error occurred: " + ex.Message, "Database Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                con.Close();
            }
        }

        private void printbtn_Click(object sender, EventArgs e)
        {
            if (PrintPreviewDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    PrintDocument.Print();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("An error occurred while printing: " + ex.Message, "Printing Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (txtBillID.Text == "")
            {
                MessageBox.Show("Missing Bill Id");
            }
            else
            {
                try
                {
                    con.Open();
                    string query = "INSERT INTO Bill (ID, Name, Date, Amount) VALUES (:ID, :Name, :Date, :Amount)";
                    OracleCommand cmd = new OracleCommand(query, con);
                    cmd.Parameters.Add(":ID", txtBillID.Text);
                    cmd.Parameters.Add(":Name", lblName.Text);
                    cmd.Parameters.Add(":Date", lblDate.Text);
                    cmd.Parameters.Add(":Amount", lblAmount.Text);
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("Order Added Successfully");
                    con.Close();
                    PopulateBills();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
        }
    }
}
