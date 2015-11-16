namespace PrimaGUI
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.VisualizzaVersione = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.button3 = new System.Windows.Forms.Button();
            this.SYNC = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Path = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Hash = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.to_fill = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Restore = new System.Windows.Forms.DataGridViewButtonColumn();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // VisualizzaVersione
            // 
            this.VisualizzaVersione.BackColor = System.Drawing.Color.Transparent;
            this.VisualizzaVersione.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.VisualizzaVersione.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.VisualizzaVersione.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.VisualizzaVersione.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.VisualizzaVersione.ForeColor = System.Drawing.Color.White;
            this.VisualizzaVersione.Location = new System.Drawing.Point(12, 9);
            this.VisualizzaVersione.Margin = new System.Windows.Forms.Padding(0);
            this.VisualizzaVersione.Name = "VisualizzaVersione";
            this.VisualizzaVersione.Size = new System.Drawing.Size(478, 37);
            this.VisualizzaVersione.TabIndex = 12;
            this.VisualizzaVersione.Text = "Visualizza Ultima Versione";
            this.VisualizzaVersione.UseCompatibleTextRendering = true;
            this.VisualizzaVersione.UseVisualStyleBackColor = false;
            this.VisualizzaVersione.Click += new System.EventHandler(this.VisualizzaVersione_Click);
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.button2.BackColor = System.Drawing.Color.Transparent;
            this.button2.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.button2.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button2.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button2.ForeColor = System.Drawing.Color.White;
            this.button2.Location = new System.Drawing.Point(492, 9);
            this.button2.Margin = new System.Windows.Forms.Padding(0);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(275, 37);
            this.button2.TabIndex = 13;
            this.button2.Text = "Cartella da sincronizzare";
            this.button2.UseCompatibleTextRendering = true;
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.Transparent;
            this.button3.FlatAppearance.BorderColor = System.Drawing.Color.Black;
            this.button3.FlatAppearance.BorderSize = 0;
            this.button3.FlatAppearance.MouseOverBackColor = System.Drawing.Color.Transparent;
            this.button3.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button3.Font = new System.Drawing.Font("Times New Roman", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button3.ForeColor = System.Drawing.Color.Transparent;
            this.button3.Location = new System.Drawing.Point(744, 374);
            this.button3.Margin = new System.Windows.Forms.Padding(0);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(10, 13);
            this.button3.TabIndex = 12;
            this.button3.UseCompatibleTextRendering = true;
            this.button3.UseVisualStyleBackColor = false;
            // 
            // SYNC
            // 
            this.SYNC.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.SYNC.BackColor = System.Drawing.Color.Transparent;
            this.SYNC.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.SYNC.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.SYNC.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.SYNC.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SYNC.ForeColor = System.Drawing.Color.White;
            this.SYNC.Location = new System.Drawing.Point(492, 413);
            this.SYNC.Margin = new System.Windows.Forms.Padding(0);
            this.SYNC.Name = "SYNC";
            this.SYNC.Size = new System.Drawing.Size(275, 37);
            this.SYNC.TabIndex = 16;
            this.SYNC.Text = "SYNC";
            this.SYNC.UseCompatibleTextRendering = true;
            this.SYNC.UseVisualStyleBackColor = false;
            this.SYNC.Click += new System.EventHandler(this.SYNC_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(8, 413);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 20);
            this.label1.TabIndex = 17;
            this.label1.Text = "label1";
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.ColumnHeader;
            this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Path,
            this.Hash,
            this.to_fill,
            this.Restore});
            this.dataGridView1.GridColor = System.Drawing.SystemColors.ActiveCaption;
            this.dataGridView1.Location = new System.Drawing.Point(12, 55);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(0, 0, 0, 0);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.ShowCellErrors = false;
            this.dataGridView1.ShowCellToolTips = false;
            this.dataGridView1.ShowEditingIcon = false;
            this.dataGridView1.ShowRowErrors = false;
            this.dataGridView1.Size = new System.Drawing.Size(755, 344);
            this.dataGridView1.TabIndex = 18;
            this.dataGridView1.Visible = false;
            this.dataGridView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            // 
            // Path
            // 
            this.Path.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Path.HeaderText = "Path";
            this.Path.Name = "Path";
            this.Path.ReadOnly = true;
            this.Path.Width = 54;
            // 
            // Hash
            // 
            this.Hash.HeaderText = "Hash";
            this.Hash.Name = "Hash";
            this.Hash.ReadOnly = true;
            this.Hash.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.Hash.Visible = false;
            this.Hash.Width = 38;
            // 
            // to_fill
            // 
            this.to_fill.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.to_fill.HeaderText = "";
            this.to_fill.MinimumWidth = 2;
            this.to_fill.Name = "to_fill";
            this.to_fill.ReadOnly = true;
            this.to_fill.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            // 
            // Restore
            // 
            this.Restore.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Restore.HeaderText = "";
            this.Restore.MinimumWidth = 70;
            this.Restore.Name = "Restore";
            this.Restore.ReadOnly = true;
            this.Restore.Text = "";
            this.Restore.Width = 70;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.BackgroundImage = global::PrimaGUI.Properties.Resources.green_blue_nature_spring_plants_macro_depth_of_field_buds_2560x1600_wallpaper_www_wallpaperhi_com_39;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(784, 462);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SYNC);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.VisualizzaVersione);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button VisualizzaVersione;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button SYNC;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Path;
        private System.Windows.Forms.DataGridViewTextBoxColumn Hash;
        private System.Windows.Forms.DataGridViewTextBoxColumn to_fill;
        private System.Windows.Forms.DataGridViewButtonColumn Restore;
    }
}

