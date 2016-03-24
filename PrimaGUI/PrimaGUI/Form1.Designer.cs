namespace PrimaGUI
{
    partial class FinestraPrincipale
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FinestraPrincipale));
            this.VisualizzaVersione = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.button3 = new System.Windows.Forms.Button();
            this.SYNC = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Path = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Hash = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Data_Ceazione = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tofill = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Restore = new System.Windows.Forms.DataGridViewButtonColumn();
            this.VisualizzaFile = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.progress = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // VisualizzaVersione
            // 
            this.VisualizzaVersione.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.VisualizzaVersione.BackColor = System.Drawing.Color.Transparent;
            this.VisualizzaVersione.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.VisualizzaVersione.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.VisualizzaVersione.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.VisualizzaVersione.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.VisualizzaVersione.ForeColor = System.Drawing.Color.White;
            this.VisualizzaVersione.Location = new System.Drawing.Point(231, 413);
            this.VisualizzaVersione.Name = "VisualizzaVersione";
            this.VisualizzaVersione.Size = new System.Drawing.Size(317, 37);
            this.VisualizzaVersione.TabIndex = 12;
            this.VisualizzaVersione.Text = "Ripristina Ultima Versione";
            this.VisualizzaVersione.UseCompatibleTextRendering = true;
            this.VisualizzaVersione.UseVisualStyleBackColor = false;
            this.VisualizzaVersione.Click += new System.EventHandler(this.RipristinaUltimaVersione_Click);
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button2.BackColor = System.Drawing.Color.Transparent;
            this.button2.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.button2.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button2.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button2.ForeColor = System.Drawing.Color.White;
            this.button2.Location = new System.Drawing.Point(502, 9);
            this.button2.Margin = new System.Windows.Forms.Padding(0);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(265, 37);
            this.button2.TabIndex = 13;
            this.button2.Text = "Seleziona la Cartella ";
            this.button2.UseCompatibleTextRendering = true;
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.SelectPath_Click);
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
            this.SYNC.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SYNC.BackColor = System.Drawing.Color.Transparent;
            this.SYNC.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.SYNC.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.SYNC.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.SYNC.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SYNC.ForeColor = System.Drawing.Color.White;
            this.SYNC.Location = new System.Drawing.Point(284, 9);
            this.SYNC.Margin = new System.Windows.Forms.Padding(0);
            this.SYNC.Name = "SYNC";
            this.SYNC.Size = new System.Drawing.Size(212, 37);
            this.SYNC.TabIndex = 16;
            this.SYNC.Text = "Sincronizza";
            this.SYNC.UseCompatibleTextRendering = true;
            this.SYNC.UseVisualStyleBackColor = false;
            this.SYNC.Click += new System.EventHandler(this.SYNC_Click);
            this.SYNC.MouseEnter += new System.EventHandler(this.SYNC_MouseEnter);
            this.SYNC.MouseLeave += new System.EventHandler(this.SYNC_MouseLeave);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridView1.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.ColumnHeader;
            this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.ActiveCaption;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Path,
            this.Hash,
            this.Data_Ceazione,
            this.tofill,
            this.Restore});
            this.dataGridView1.GridColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.dataGridView1.Location = new System.Drawing.Point(12, 55);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(0);
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
            this.Hash.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Hash.HeaderText = "Hash";
            this.Hash.Name = "Hash";
            this.Hash.ReadOnly = true;
            this.Hash.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.Hash.Visible = false;
            // 
            // Data_Ceazione
            // 
            this.Data_Ceazione.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.Data_Ceazione.HeaderText = "Data";
            this.Data_Ceazione.MinimumWidth = 2;
            this.Data_Ceazione.Name = "Data_Ceazione";
            this.Data_Ceazione.ReadOnly = true;
            this.Data_Ceazione.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.NotSortable;
            this.Data_Ceazione.Width = 36;
            // 
            // tofill
            // 
            this.tofill.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.tofill.HeaderText = "";
            this.tofill.MinimumWidth = 2;
            this.tofill.Name = "tofill";
            this.tofill.ReadOnly = true;
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
            // VisualizzaFile
            // 
            this.VisualizzaFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.VisualizzaFile.BackColor = System.Drawing.Color.Transparent;
            this.VisualizzaFile.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.VisualizzaFile.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.VisualizzaFile.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.VisualizzaFile.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.VisualizzaFile.ForeColor = System.Drawing.Color.White;
            this.VisualizzaFile.Location = new System.Drawing.Point(12, 413);
            this.VisualizzaFile.Name = "VisualizzaFile";
            this.VisualizzaFile.Size = new System.Drawing.Size(213, 37);
            this.VisualizzaFile.TabIndex = 19;
            this.VisualizzaFile.Text = "Tutti i Files";
            this.VisualizzaFile.UseCompatibleTextRendering = true;
            this.VisualizzaFile.UseVisualStyleBackColor = false;
            this.VisualizzaFile.Click += new System.EventHandler(this.VisualizzaFile_Click);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.BackColor = System.Drawing.Color.Transparent;
            this.button1.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.button1.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button1.Font = new System.Drawing.Font("Times New Roman", 17F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.ForeColor = System.Drawing.Color.White;
            this.button1.Location = new System.Drawing.Point(554, 413);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(213, 37);
            this.button1.TabIndex = 20;
            this.button1.Text = "Visualizza Versioni";
            this.button1.UseCompatibleTextRendering = true;
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.VisualizzaVersioni_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 100000;
            this.timer1.Tick += new System.EventHandler(this.SYNC_Timer);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Font = new System.Drawing.Font("Times New Roman", 27.75F, System.Drawing.FontStyle.Bold);
            this.label2.ForeColor = System.Drawing.Color.Teal;
            this.label2.Location = new System.Drawing.Point(25, 1);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(129, 42);
            this.label2.TabIndex = 21;
            this.label2.Text = "GEMS";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Font = new System.Drawing.Font("Times New Roman", 8.75F, System.Drawing.FontStyle.Bold);
            this.label3.ForeColor = System.Drawing.Color.Teal;
            this.label3.Location = new System.Drawing.Point(125, 35);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(79, 15);
            this.label3.TabIndex = 22;
            this.label3.Text = "a PDS project";
            // 
            // progress
            // 
            this.progress.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.progress.AutoSize = true;
            this.progress.BackColor = System.Drawing.Color.Transparent;
            this.progress.Font = new System.Drawing.Font("Times New Roman", 8.75F, System.Drawing.FontStyle.Bold);
            this.progress.ForeColor = System.Drawing.Color.White;
            this.progress.Location = new System.Drawing.Point(454, 26);
            this.progress.Margin = new System.Windows.Forms.Padding(0);
            this.progress.Name = "progress";
            this.progress.Size = new System.Drawing.Size(36, 15);
            this.progress.TabIndex = 23;
            this.progress.Text = "100%";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.BackgroundImage = global::PrimaGUI.Properties.Resources.green_blue_nature_spring_plants_macro_depth_of_field_buds_2560x1600_wallpaper_www_wallpaperhi_com_39;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(784, 462);
            this.Controls.Add(this.progress);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.VisualizzaFile);
            this.Controls.Add(this.dataGridView1);
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
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button VisualizzaFile;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Path;
        private System.Windows.Forms.DataGridViewTextBoxColumn Hash;
        private System.Windows.Forms.DataGridViewTextBoxColumn Data_Ceazione;
        private System.Windows.Forms.DataGridViewTextBoxColumn tofill;
        private System.Windows.Forms.DataGridViewButtonColumn Restore;
        private System.Windows.Forms.Label progress;
    }
}

