namespace Project
{
    partial class Selling
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Selling));
            this.btnCategory = new System.Windows.Forms.Button();
            this.btnProduct = new System.Windows.Forms.Button();
            this.btnSeller = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.PictureBox();
            this.btnLogout = new System.Windows.Forms.Button();
            this.Espurt = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.printbtn = new System.Windows.Forms.PictureBox();
            this.AddProductbtn = new System.Windows.Forms.Button();
            this.selectcategory = new System.Windows.Forms.ComboBox();
            this.refreshbtn = new System.Windows.Forms.Button();
            this.lblSellerList = new System.Windows.Forms.Label();
            this.Billdgv = new System.Windows.Forms.DataGridView();
            this.lblRs = new System.Windows.Forms.Label();
            this.lblAmount = new System.Windows.Forms.Label();
            this.OrderDGV = new System.Windows.Forms.DataGridView();
            this.lblDate = new System.Windows.Forms.Label();
            this.lblsellername = new System.Windows.Forms.Label();
            this.lblPrice = new System.Windows.Forms.Label();
            this.pricetxt = new System.Windows.Forms.TextBox();
            this.ProuductDGV = new System.Windows.Forms.DataGridView();
            this.btnDelete = new System.Windows.Forms.PictureBox();
            this.btnAdd = new System.Windows.Forms.PictureBox();
            this.txtQuantity = new System.Windows.Forms.TextBox();
            this.txtName = new System.Windows.Forms.TextBox();
            this.txtBillID = new System.Windows.Forms.TextBox();
            this.txtQuality = new System.Windows.Forms.Label();
            this.id = new System.Windows.Forms.MaskedTextBox();
            this.lblName = new System.Windows.Forms.Label();
            this.lblBill = new System.Windows.Forms.Label();
            this.lblSellingStaus = new System.Windows.Forms.Label();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.PrintDocument = new System.Drawing.Printing.PrintDocument();
            this.PrintPreviewDialog = new System.Windows.Forms.PrintPreviewDialog();
            ((System.ComponentModel.ISupportInitialize)(this.btnClose)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.printbtn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Billdgv)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.OrderDGV)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ProuductDGV)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnAdd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCategory
            // 
            this.btnCategory.BackColor = System.Drawing.Color.Transparent;
            this.btnCategory.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnCategory.FlatAppearance.BorderSize = 0;
            this.btnCategory.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCategory.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCategory.ForeColor = System.Drawing.Color.DarkOrange;
            this.btnCategory.Location = new System.Drawing.Point(26, 157);
            this.btnCategory.Name = "btnCategory";
            this.btnCategory.Size = new System.Drawing.Size(101, 36);
            this.btnCategory.TabIndex = 52;
            this.btnCategory.Text = "Categories";
            this.btnCategory.UseVisualStyleBackColor = false;
            this.btnCategory.Click += new System.EventHandler(this.btnCategory_Click);
            // 
            // btnProduct
            // 
            this.btnProduct.BackColor = System.Drawing.Color.Transparent;
            this.btnProduct.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnProduct.FlatAppearance.BorderSize = 0;
            this.btnProduct.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnProduct.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnProduct.ForeColor = System.Drawing.Color.DarkOrange;
            this.btnProduct.Location = new System.Drawing.Point(26, 199);
            this.btnProduct.Name = "btnProduct";
            this.btnProduct.Size = new System.Drawing.Size(101, 36);
            this.btnProduct.TabIndex = 51;
            this.btnProduct.Text = "Product";
            this.btnProduct.UseVisualStyleBackColor = false;
            this.btnProduct.Click += new System.EventHandler(this.btnProduct_Click);
            // 
            // btnSeller
            // 
            this.btnSeller.BackColor = System.Drawing.Color.Transparent;
            this.btnSeller.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSeller.FlatAppearance.BorderSize = 0;
            this.btnSeller.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSeller.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSeller.ForeColor = System.Drawing.Color.DarkOrange;
            this.btnSeller.Location = new System.Drawing.Point(26, 115);
            this.btnSeller.Name = "btnSeller";
            this.btnSeller.Size = new System.Drawing.Size(101, 36);
            this.btnSeller.TabIndex = 50;
            this.btnSeller.Text = "Seller";
            this.btnSeller.UseVisualStyleBackColor = false;
            this.btnSeller.Click += new System.EventHandler(this.productbtn_Click);
            // 
            // btnClose
            // 
            this.btnClose.BackColor = System.Drawing.Color.Transparent;
            this.btnClose.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnClose.Image = ((System.Drawing.Image)(resources.GetObject("btnClose.Image")));
            this.btnClose.Location = new System.Drawing.Point(866, 1);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(30, 30);
            this.btnClose.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.btnClose.TabIndex = 49;
            this.btnClose.TabStop = false;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnLogout
            // 
            this.btnLogout.BackColor = System.Drawing.Color.Transparent;
            this.btnLogout.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnLogout.FlatAppearance.BorderSize = 0;
            this.btnLogout.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLogout.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLogout.ForeColor = System.Drawing.Color.DarkOrange;
            this.btnLogout.Location = new System.Drawing.Point(26, 525);
            this.btnLogout.Name = "btnLogout";
            this.btnLogout.Size = new System.Drawing.Size(101, 36);
            this.btnLogout.TabIndex = 53;
            this.btnLogout.Text = "Logout";
            this.btnLogout.UseVisualStyleBackColor = false;
            this.btnLogout.Click += new System.EventHandler(this.btnLogout_Click);
            // 
            // Espurt
            // 
            this.Espurt.AutoSize = true;
            this.Espurt.Font = new System.Drawing.Font("Century Gothic", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Espurt.Location = new System.Drawing.Point(51, 21);
            this.Espurt.Name = "Espurt";
            this.Espurt.Size = new System.Drawing.Size(118, 38);
            this.Espurt.TabIndex = 48;
            this.Espurt.Text = "Selling";
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.DarkOrange;
            this.panel1.Controls.Add(this.printbtn);
            this.panel1.Controls.Add(this.AddProductbtn);
            this.panel1.Controls.Add(this.selectcategory);
            this.panel1.Controls.Add(this.refreshbtn);
            this.panel1.Controls.Add(this.lblSellerList);
            this.panel1.Controls.Add(this.Billdgv);
            this.panel1.Controls.Add(this.lblRs);
            this.panel1.Controls.Add(this.lblAmount);
            this.panel1.Controls.Add(this.OrderDGV);
            this.panel1.Controls.Add(this.lblDate);
            this.panel1.Controls.Add(this.lblsellername);
            this.panel1.Controls.Add(this.lblPrice);
            this.panel1.Controls.Add(this.pricetxt);
            this.panel1.Controls.Add(this.ProuductDGV);
            this.panel1.Controls.Add(this.btnDelete);
            this.panel1.Controls.Add(this.btnAdd);
            this.panel1.Controls.Add(this.txtQuantity);
            this.panel1.Controls.Add(this.txtName);
            this.panel1.Controls.Add(this.txtBillID);
            this.panel1.Controls.Add(this.txtQuality);
            this.panel1.Controls.Add(this.id);
            this.panel1.Controls.Add(this.lblName);
            this.panel1.Controls.Add(this.lblBill);
            this.panel1.Controls.Add(this.lblSellingStaus);
            this.panel1.Location = new System.Drawing.Point(143, 62);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(771, 532);
            this.panel1.TabIndex = 47;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // printbtn
            // 
            this.printbtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.printbtn.Image = ((System.Drawing.Image)(resources.GetObject("printbtn.Image")));
            this.printbtn.Location = new System.Drawing.Point(508, 466);
            this.printbtn.Name = "printbtn";
            this.printbtn.Size = new System.Drawing.Size(90, 39);
            this.printbtn.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.printbtn.TabIndex = 47;
            this.printbtn.TabStop = false;
            this.printbtn.Click += new System.EventHandler(this.printbtn_Click);
            // 
            // AddProductbtn
            // 
            this.AddProductbtn.BackColor = System.Drawing.Color.White;
            this.AddProductbtn.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.AddProductbtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.AddProductbtn.FlatAppearance.BorderColor = System.Drawing.Color.WhiteSmoke;
            this.AddProductbtn.FlatAppearance.BorderSize = 0;
            this.AddProductbtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.AddProductbtn.Font = new System.Drawing.Font("Century Gothic", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.AddProductbtn.ForeColor = System.Drawing.Color.DarkOrange;
            this.AddProductbtn.Location = new System.Drawing.Point(88, 198);
            this.AddProductbtn.Name = "AddProductbtn";
            this.AddProductbtn.Size = new System.Drawing.Size(127, 23);
            this.AddProductbtn.TabIndex = 46;
            this.AddProductbtn.Text = "Add Product";
            this.AddProductbtn.UseVisualStyleBackColor = false;
            this.AddProductbtn.Click += new System.EventHandler(this.AddProductbtn_Click);
            // 
            // selectcategory
            // 
            this.selectcategory.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.selectcategory.ForeColor = System.Drawing.Color.DarkOrange;
            this.selectcategory.FormattingEnabled = true;
            this.selectcategory.Items.AddRange(new object[] {
            "Admin",
            "Manager"});
            this.selectcategory.Location = new System.Drawing.Point(29, 236);
            this.selectcategory.Name = "selectcategory";
            this.selectcategory.Size = new System.Drawing.Size(201, 27);
            this.selectcategory.TabIndex = 45;
            this.selectcategory.Text = "Select Category";
            this.selectcategory.SelectionChangeCommitted += new System.EventHandler(this.selectcategory_SelectionChangeCommitted);
            // 
            // refreshbtn
            // 
            this.refreshbtn.BackColor = System.Drawing.Color.White;
            this.refreshbtn.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.refreshbtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.refreshbtn.FlatAppearance.BorderColor = System.Drawing.Color.WhiteSmoke;
            this.refreshbtn.FlatAppearance.BorderSize = 0;
            this.refreshbtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.refreshbtn.Font = new System.Drawing.Font("Century Gothic", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.refreshbtn.ForeColor = System.Drawing.Color.DarkOrange;
            this.refreshbtn.Location = new System.Drawing.Point(236, 238);
            this.refreshbtn.Name = "refreshbtn";
            this.refreshbtn.Size = new System.Drawing.Size(75, 23);
            this.refreshbtn.TabIndex = 44;
            this.refreshbtn.Text = "Refresh";
            this.refreshbtn.UseVisualStyleBackColor = false;
            this.refreshbtn.Click += new System.EventHandler(this.refreshbtn_Click);
            // 
            // lblSellerList
            // 
            this.lblSellerList.AutoSize = true;
            this.lblSellerList.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSellerList.ForeColor = System.Drawing.Color.White;
            this.lblSellerList.Location = new System.Drawing.Point(466, 267);
            this.lblSellerList.Name = "lblSellerList";
            this.lblSellerList.Size = new System.Drawing.Size(82, 23);
            this.lblSellerList.TabIndex = 43;
            this.lblSellerList.Text = "Sells List";
            // 
            // Billdgv
            // 
            this.Billdgv.BackgroundColor = System.Drawing.Color.White;
            this.Billdgv.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.Billdgv.Location = new System.Drawing.Point(320, 293);
            this.Billdgv.Name = "Billdgv";
            this.Billdgv.Size = new System.Drawing.Size(433, 167);
            this.Billdgv.TabIndex = 42;
            // 
            // lblRs
            // 
            this.lblRs.AutoSize = true;
            this.lblRs.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRs.ForeColor = System.Drawing.Color.White;
            this.lblRs.Location = new System.Drawing.Point(544, 210);
            this.lblRs.Name = "lblRs";
            this.lblRs.Size = new System.Drawing.Size(29, 23);
            this.lblRs.TabIndex = 41;
            this.lblRs.Text = "Rs";
            // 
            // lblAmount
            // 
            this.lblAmount.AutoSize = true;
            this.lblAmount.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblAmount.ForeColor = System.Drawing.Color.White;
            this.lblAmount.Location = new System.Drawing.Point(408, 210);
            this.lblAmount.Name = "lblAmount";
            this.lblAmount.Size = new System.Drawing.Size(106, 23);
            this.lblAmount.TabIndex = 40;
            this.lblAmount.Text = "Amount Rs";
            // 
            // OrderDGV
            // 
            this.OrderDGV.BackgroundColor = System.Drawing.Color.White;
            this.OrderDGV.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.OrderDGV.Location = new System.Drawing.Point(29, 267);
            this.OrderDGV.Name = "OrderDGV";
            this.OrderDGV.Size = new System.Drawing.Size(282, 193);
            this.OrderDGV.TabIndex = 39;
            this.OrderDGV.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.productdgv_CellContentClick);
            // 
            // lblDate
            // 
            this.lblDate.AutoSize = true;
            this.lblDate.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDate.ForeColor = System.Drawing.Color.White;
            this.lblDate.Location = new System.Drawing.Point(600, 9);
            this.lblDate.Name = "lblDate";
            this.lblDate.Size = new System.Drawing.Size(54, 23);
            this.lblDate.TabIndex = 38;
            this.lblDate.Text = "Date";
            // 
            // lblsellername
            // 
            this.lblsellername.AutoSize = true;
            this.lblsellername.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblsellername.ForeColor = System.Drawing.Color.White;
            this.lblsellername.Location = new System.Drawing.Point(3, 0);
            this.lblsellername.Name = "lblsellername";
            this.lblsellername.Size = new System.Drawing.Size(122, 23);
            this.lblsellername.TabIndex = 37;
            this.lblsellername.Text = "Seller Name";
            // 
            // lblPrice
            // 
            this.lblPrice.AutoSize = true;
            this.lblPrice.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblPrice.ForeColor = System.Drawing.Color.White;
            this.lblPrice.Location = new System.Drawing.Point(25, 156);
            this.lblPrice.Name = "lblPrice";
            this.lblPrice.Size = new System.Drawing.Size(47, 19);
            this.lblPrice.TabIndex = 31;
            this.lblPrice.Text = "Price";
            // 
            // pricetxt
            // 
            this.pricetxt.Location = new System.Drawing.Point(125, 156);
            this.pricetxt.Name = "pricetxt";
            this.pricetxt.Size = new System.Drawing.Size(166, 27);
            this.pricetxt.TabIndex = 30;
            // 
            // ProuductDGV
            // 
            this.ProuductDGV.BackgroundColor = System.Drawing.Color.White;
            this.ProuductDGV.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.ProuductDGV.Location = new System.Drawing.Point(317, 43);
            this.ProuductDGV.Name = "ProuductDGV";
            this.ProuductDGV.Size = new System.Drawing.Size(433, 152);
            this.ProuductDGV.TabIndex = 29;
            // 
            // btnDelete
            // 
            this.btnDelete.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnDelete.Image = ((System.Drawing.Image)(resources.GetObject("btnDelete.Image")));
            this.btnDelete.Location = new System.Drawing.Point(604, 466);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(90, 39);
            this.btnDelete.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.btnDelete.TabIndex = 28;
            this.btnDelete.TabStop = false;
            // 
            // btnAdd
            // 
            this.btnAdd.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnAdd.Image = ((System.Drawing.Image)(resources.GetObject("btnAdd.Image")));
            this.btnAdd.Location = new System.Drawing.Point(412, 466);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(90, 39);
            this.btnAdd.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.btnAdd.TabIndex = 26;
            this.btnAdd.TabStop = false;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // txtQuantity
            // 
            this.txtQuantity.Location = new System.Drawing.Point(125, 127);
            this.txtQuantity.Name = "txtQuantity";
            this.txtQuantity.Size = new System.Drawing.Size(166, 27);
            this.txtQuantity.TabIndex = 25;
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(125, 98);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(166, 27);
            this.txtName.TabIndex = 24;
            // 
            // txtBillID
            // 
            this.txtBillID.Location = new System.Drawing.Point(125, 69);
            this.txtBillID.Name = "txtBillID";
            this.txtBillID.Size = new System.Drawing.Size(166, 27);
            this.txtBillID.TabIndex = 23;
            // 
            // txtQuality
            // 
            this.txtQuality.AutoSize = true;
            this.txtQuality.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtQuality.ForeColor = System.Drawing.Color.White;
            this.txtQuality.Location = new System.Drawing.Point(25, 127);
            this.txtQuality.Name = "txtQuality";
            this.txtQuality.Size = new System.Drawing.Size(64, 19);
            this.txtQuality.TabIndex = 22;
            this.txtQuality.Text = "Quality";
            // 
            // id
            // 
            this.id.BackColor = System.Drawing.Color.DarkOrange;
            this.id.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.id.ForeColor = System.Drawing.Color.White;
            this.id.Location = new System.Drawing.Point(125, 109);
            this.id.Name = "id";
            this.id.Size = new System.Drawing.Size(100, 20);
            this.id.TabIndex = 21;
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblName.ForeColor = System.Drawing.Color.White;
            this.lblName.Location = new System.Drawing.Point(25, 98);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(58, 19);
            this.lblName.TabIndex = 20;
            this.lblName.Text = "Name";
            // 
            // lblBill
            // 
            this.lblBill.AutoSize = true;
            this.lblBill.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblBill.ForeColor = System.Drawing.Color.White;
            this.lblBill.Location = new System.Drawing.Point(25, 69);
            this.lblBill.Name = "lblBill";
            this.lblBill.Size = new System.Drawing.Size(49, 19);
            this.lblBill.TabIndex = 15;
            this.lblBill.Text = "Bill ID";
            // 
            // lblSellingStaus
            // 
            this.lblSellingStaus.AutoSize = true;
            this.lblSellingStaus.Font = new System.Drawing.Font("Century Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSellingStaus.ForeColor = System.Drawing.Color.White;
            this.lblSellingStaus.Location = new System.Drawing.Point(408, 9);
            this.lblSellingStaus.Name = "lblSellingStaus";
            this.lblSellingStaus.Size = new System.Drawing.Size(130, 23);
            this.lblSellingStaus.TabIndex = 14;
            this.lblSellingStaus.Text = "Selling Status";
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox4.Image")));
            this.pictureBox4.Location = new System.Drawing.Point(0, 21);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(59, 50);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox4.TabIndex = 54;
            this.pictureBox4.TabStop = false;
            // 
            // PrintDocument
            // 
            this.PrintDocument.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(this.PrintDocument_PrintPage);
            // 
            // PrintPreviewDialog
            // 
            this.PrintPreviewDialog.AutoScrollMargin = new System.Drawing.Size(0, 0);
            this.PrintPreviewDialog.AutoScrollMinSize = new System.Drawing.Size(0, 0);
            this.PrintPreviewDialog.ClientSize = new System.Drawing.Size(400, 300);
            this.PrintPreviewDialog.Enabled = true;
            this.PrintPreviewDialog.Icon = ((System.Drawing.Icon)(resources.GetObject("PrintPreviewDialog.Icon")));
            this.PrintPreviewDialog.Name = "PrintPreviewDialog";
            this.PrintPreviewDialog.Visible = false;
            // 
            // Selling
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(915, 600);
            this.Controls.Add(this.btnCategory);
            this.Controls.Add(this.btnProduct);
            this.Controls.Add(this.btnSeller);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnLogout);
            this.Controls.Add(this.Espurt);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.pictureBox4);
            this.Font = new System.Drawing.Font("Century Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(5);
            this.Name = "Selling";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Selling";
            this.Load += new System.EventHandler(this.Selling_Load);
            ((System.ComponentModel.ISupportInitialize)(this.btnClose)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.printbtn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Billdgv)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.OrderDGV)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ProuductDGV)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnAdd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button btnCategory;
        private System.Windows.Forms.Button btnProduct;
        private System.Windows.Forms.Button btnSeller;
        private System.Windows.Forms.PictureBox btnClose;
        private System.Windows.Forms.Button btnLogout;
        private System.Windows.Forms.Label Espurt;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lblPrice;
        private System.Windows.Forms.TextBox pricetxt;
        private System.Windows.Forms.DataGridView ProuductDGV;
        private System.Windows.Forms.PictureBox btnDelete;
        private System.Windows.Forms.PictureBox btnAdd;
        private System.Windows.Forms.TextBox txtQuantity;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.TextBox txtBillID;
        private System.Windows.Forms.Label txtQuality;
        private System.Windows.Forms.MaskedTextBox id;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.Label lblBill;
        private System.Windows.Forms.Label lblSellingStaus;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.Label lblsellername;
        private System.Windows.Forms.Label lblDate;
        private System.Windows.Forms.Label lblSellerList;
        private System.Windows.Forms.DataGridView Billdgv;
        private System.Windows.Forms.Label lblRs;
        private System.Windows.Forms.Label lblAmount;
        private System.Windows.Forms.DataGridView OrderDGV;
        private System.Windows.Forms.Button refreshbtn;
        private System.Windows.Forms.PictureBox printbtn;
        private System.Windows.Forms.Button AddProductbtn;
        private System.Windows.Forms.ComboBox selectcategory;
        private System.Drawing.Printing.PrintDocument PrintDocument;
        private System.Windows.Forms.PrintPreviewDialog PrintPreviewDialog;
    }
}