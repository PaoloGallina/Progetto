namespace PrimaGUI
{
    partial class FinestraLogin
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FinestraLogin));
            this.LoginButton = new System.Windows.Forms.Button();
            this.RegisterButton = new System.Windows.Forms.Button();
            this.UsernameText = new System.Windows.Forms.TextBox();
            this.PasswordText = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.OkButton = new System.Windows.Forms.Button();
            this.IndietroButton = new System.Windows.Forms.Button();
            this.CartDaSyncButton = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.label4 = new System.Windows.Forms.Label();
            this.HelpButtonN = new System.Windows.Forms.Button();
            this.AboutButton = new System.Windows.Forms.Button();
            this.infoLabel = new System.Windows.Forms.Label();
            this.IpAddressText = new System.Windows.Forms.TextBox();
            this.titolo = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.PortaText = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // LoginButton
            // 
            this.LoginButton.BackColor = System.Drawing.Color.Transparent;
            this.LoginButton.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.LoginButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.LoginButton.Font = new System.Drawing.Font("Times New Roman", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LoginButton.ForeColor = System.Drawing.Color.White;
            this.LoginButton.Location = new System.Drawing.Point(90, 274);
            this.LoginButton.Name = "LoginButton";
            this.LoginButton.Size = new System.Drawing.Size(121, 42);
            this.LoginButton.TabIndex = 5;
            this.LoginButton.Text = "Login";
            this.LoginButton.UseVisualStyleBackColor = false;
            this.LoginButton.Click += new System.EventHandler(this.Login_Click);
            // 
            // RegisterButton
            // 
            this.RegisterButton.BackColor = System.Drawing.Color.Transparent;
            this.RegisterButton.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.RegisterButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.RegisterButton.Font = new System.Drawing.Font("Times New Roman", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.RegisterButton.ForeColor = System.Drawing.Color.White;
            this.RegisterButton.Location = new System.Drawing.Point(90, 209);
            this.RegisterButton.Name = "RegisterButton";
            this.RegisterButton.Size = new System.Drawing.Size(122, 42);
            this.RegisterButton.TabIndex = 4;
            this.RegisterButton.Text = "Registrati";
            this.RegisterButton.UseVisualStyleBackColor = false;
            this.RegisterButton.Click += new System.EventHandler(this.register_Click);
            // 
            // UsernameText
            // 
            this.UsernameText.BackColor = System.Drawing.SystemColors.InactiveBorder;
            this.UsernameText.Font = new System.Drawing.Font("Arial", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.UsernameText.ForeColor = System.Drawing.SystemColors.InactiveCaption;
            this.UsernameText.Location = new System.Drawing.Point(11, 206);
            this.UsernameText.MaxLength = 40;
            this.UsernameText.Name = "UsernameText";
            this.UsernameText.Size = new System.Drawing.Size(278, 25);
            this.UsernameText.TabIndex = 0;
            this.UsernameText.Text = "Username";
            this.UsernameText.Visible = false;
            this.UsernameText.MouseClick += new System.Windows.Forms.MouseEventHandler(this.UserText_MouseClick);
            this.UsernameText.TextChanged += new System.EventHandler(this.UserText_TextChanged);
            this.UsernameText.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ControlloEnter_KeyPress);
            // 
            // PasswordText
            // 
            this.PasswordText.BackColor = System.Drawing.SystemColors.InactiveBorder;
            this.PasswordText.Font = new System.Drawing.Font("Arial", 10.25F);
            this.PasswordText.ForeColor = System.Drawing.SystemColors.InactiveCaption;
            this.PasswordText.Location = new System.Drawing.Point(11, 237);
            this.PasswordText.MaxLength = 40;
            this.PasswordText.Name = "PasswordText";
            this.PasswordText.Size = new System.Drawing.Size(278, 23);
            this.PasswordText.TabIndex = 1;
            this.PasswordText.Text = "Password";
            this.PasswordText.Visible = false;
            this.PasswordText.MouseClick += new System.Windows.Forms.MouseEventHandler(this.PassText_MouseClick);
            this.PasswordText.TextChanged += new System.EventHandler(this.PassText_TextChanged);
            this.PasswordText.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ControlloEnter_KeyPress);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.SystemColors.HighlightText;
            this.label3.Font = new System.Drawing.Font("Arial", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(5, 9);
            this.label3.MaximumSize = new System.Drawing.Size(180, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(0, 19);
            this.label3.TabIndex = 6;
            // 
            // OkButton
            // 
            this.OkButton.BackColor = System.Drawing.Color.Transparent;
            this.OkButton.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.OkButton.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.OkButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.OkButton.Font = new System.Drawing.Font("Times New Roman", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.OkButton.ForeColor = System.Drawing.Color.White;
            this.OkButton.Location = new System.Drawing.Point(154, 377);
            this.OkButton.Name = "OkButton";
            this.OkButton.Size = new System.Drawing.Size(137, 27);
            this.OkButton.TabIndex = 7;
            this.OkButton.Text = "OK";
            this.OkButton.UseCompatibleTextRendering = true;
            this.OkButton.UseVisualStyleBackColor = false;
            this.OkButton.Visible = false;
            this.OkButton.Click += new System.EventHandler(this.OK_Click);
            // 
            // IndietroButton
            // 
            this.IndietroButton.BackColor = System.Drawing.Color.Transparent;
            this.IndietroButton.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.IndietroButton.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.IndietroButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.IndietroButton.Font = new System.Drawing.Font("Times New Roman", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.IndietroButton.ForeColor = System.Drawing.Color.White;
            this.IndietroButton.Location = new System.Drawing.Point(9, 377);
            this.IndietroButton.Name = "IndietroButton";
            this.IndietroButton.Size = new System.Drawing.Size(137, 27);
            this.IndietroButton.TabIndex = 8;
            this.IndietroButton.Text = "INDIETRO";
            this.IndietroButton.UseCompatibleTextRendering = true;
            this.IndietroButton.UseVisualStyleBackColor = false;
            this.IndietroButton.Visible = false;
            this.IndietroButton.Click += new System.EventHandler(this.Indietro_Click);
            // 
            // CartDaSyncButton
            // 
            this.CartDaSyncButton.BackColor = System.Drawing.Color.Transparent;
            this.CartDaSyncButton.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.CartDaSyncButton.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.CartDaSyncButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CartDaSyncButton.Font = new System.Drawing.Font("Times New Roman", 13.25F);
            this.CartDaSyncButton.ForeColor = System.Drawing.SystemColors.ButtonHighlight;
            this.CartDaSyncButton.Location = new System.Drawing.Point(90, 325);
            this.CartDaSyncButton.Name = "CartDaSyncButton";
            this.CartDaSyncButton.Size = new System.Drawing.Size(121, 49);
            this.CartDaSyncButton.TabIndex = 6;
            this.CartDaSyncButton.Text = "Cartella da sincronizzare";
            this.CartDaSyncButton.UseCompatibleTextRendering = true;
            this.CartDaSyncButton.UseVisualStyleBackColor = false;
            this.CartDaSyncButton.Visible = false;
            this.CartDaSyncButton.Click += new System.EventHandler(this.ChooseFolder_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.Transparent;
            this.label4.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.label4.Font = new System.Drawing.Font("Times New Roman", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(7, 180);
            this.label4.MaximumSize = new System.Drawing.Size(320, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(177, 22);
            this.label4.TabIndex = 10;
            this.label4.Text = "Inserisci tutti i campi";
            this.label4.Visible = false;
            // 
            // HelpButtonN
            // 
            this.HelpButtonN.BackColor = System.Drawing.Color.Transparent;
            this.HelpButtonN.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.HelpButtonN.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.HelpButtonN.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.HelpButtonN.Font = new System.Drawing.Font("Times New Roman", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.HelpButtonN.ForeColor = System.Drawing.Color.White;
            this.HelpButtonN.Location = new System.Drawing.Point(179, 377);
            this.HelpButtonN.Name = "HelpButtonN";
            this.HelpButtonN.Size = new System.Drawing.Size(112, 27);
            this.HelpButtonN.TabIndex = 11;
            this.HelpButtonN.Text = "Help";
            this.HelpButtonN.UseCompatibleTextRendering = true;
            this.HelpButtonN.UseVisualStyleBackColor = false;
            this.HelpButtonN.Click += new System.EventHandler(this.Help_Click);
            // 
            // AboutButton
            // 
            this.AboutButton.BackColor = System.Drawing.Color.Transparent;
            this.AboutButton.FlatAppearance.BorderColor = System.Drawing.Color.White;
            this.AboutButton.FlatAppearance.MouseOverBackColor = System.Drawing.SystemColors.ActiveCaption;
            this.AboutButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.AboutButton.Font = new System.Drawing.Font("Times New Roman", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.AboutButton.ForeColor = System.Drawing.Color.White;
            this.AboutButton.Location = new System.Drawing.Point(9, 377);
            this.AboutButton.Name = "AboutButton";
            this.AboutButton.Size = new System.Drawing.Size(112, 27);
            this.AboutButton.TabIndex = 12;
            this.AboutButton.Text = "About";
            this.AboutButton.UseCompatibleTextRendering = true;
            this.AboutButton.UseVisualStyleBackColor = false;
            this.AboutButton.Click += new System.EventHandler(this.About_Click);
            // 
            // infoLabel
            // 
            this.infoLabel.AutoSize = true;
            this.infoLabel.BackColor = System.Drawing.Color.Transparent;
            this.infoLabel.Font = new System.Drawing.Font("Arial", 12.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.infoLabel.ForeColor = System.Drawing.SystemColors.ControlText;
            this.infoLabel.Location = new System.Drawing.Point(7, 187);
            this.infoLabel.MaximumSize = new System.Drawing.Size(300, 0);
            this.infoLabel.Name = "infoLabel";
            this.infoLabel.Size = new System.Drawing.Size(81, 19);
            this.infoLabel.TabIndex = 13;
            this.infoLabel.Text = "About&help";
            this.infoLabel.Visible = false;
            // 
            // IpAddressText
            // 
            this.IpAddressText.BackColor = System.Drawing.SystemColors.InactiveBorder;
            this.IpAddressText.Font = new System.Drawing.Font("Arial", 10.25F);
            this.IpAddressText.ForeColor = System.Drawing.SystemColors.InactiveCaption;
            this.IpAddressText.Location = new System.Drawing.Point(11, 267);
            this.IpAddressText.MaxLength = 15;
            this.IpAddressText.Name = "IpAddressText";
            this.IpAddressText.Size = new System.Drawing.Size(278, 23);
            this.IpAddressText.TabIndex = 2;
            this.IpAddressText.Text = "IpAddress";
            this.IpAddressText.Visible = false;
            this.IpAddressText.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Iptext_MouseClick);
            this.IpAddressText.TextChanged += new System.EventHandler(this.Iptext_TextChanged);
            this.IpAddressText.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ControlloEnter_KeyPress);
            // 
            // titolo
            // 
            this.titolo.AutoSize = true;
            this.titolo.BackColor = System.Drawing.Color.Transparent;
            this.titolo.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.titolo.Font = new System.Drawing.Font("Impact", 25F);
            this.titolo.ForeColor = System.Drawing.Color.White;
            this.titolo.Location = new System.Drawing.Point(172, 9);
            this.titolo.MaximumSize = new System.Drawing.Size(320, 0);
            this.titolo.Name = "titolo";
            this.titolo.Size = new System.Drawing.Size(93, 42);
            this.titolo.TabIndex = 14;
            this.titolo.Text = "GEMS\n";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Times New Roman", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(213, 44);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 15);
            this.label1.TabIndex = 15;
            this.label1.Text = "a PDS project";
            // 
            // PortaText
            // 
            this.PortaText.BackColor = System.Drawing.SystemColors.InactiveBorder;
            this.PortaText.Font = new System.Drawing.Font("Arial", 10.25F);
            this.PortaText.ForeColor = System.Drawing.SystemColors.InactiveCaption;
            this.PortaText.Location = new System.Drawing.Point(11, 296);
            this.PortaText.MaxLength = 15;
            this.PortaText.Name = "PortaText";
            this.PortaText.Size = new System.Drawing.Size(278, 23);
            this.PortaText.TabIndex = 3;
            this.PortaText.Text = "Porta";
            this.PortaText.Visible = false;
            this.PortaText.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Porta_MouseClick);
            this.PortaText.TextChanged += new System.EventHandler(this.Porta_TextChanged);
            this.PortaText.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ControlloEnter_KeyPress);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::PrimaGUI.Properties.Resources.b;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(304, 416);
            this.Controls.Add(this.PortaText);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.titolo);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.IpAddressText);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.CartDaSyncButton);
            this.Controls.Add(this.PasswordText);
            this.Controls.Add(this.UsernameText);
            this.Controls.Add(this.LoginButton);
            this.Controls.Add(this.RegisterButton);
            this.Controls.Add(this.HelpButtonN);
            this.Controls.Add(this.infoLabel);
            this.Controls.Add(this.AboutButton);
            this.Controls.Add(this.OkButton);
            this.Controls.Add(this.IndietroButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form2";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ControlloEnter_KeyPress);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel2_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panel2_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panel2_MouseUp);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button LoginButton;
        private System.Windows.Forms.Button RegisterButton;
        private System.Windows.Forms.TextBox UsernameText;
        private System.Windows.Forms.TextBox PasswordText;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button OkButton;
        private System.Windows.Forms.Button IndietroButton;
        private System.Windows.Forms.Button CartDaSyncButton;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button HelpButtonN;
        private System.Windows.Forms.Button AboutButton;
        private System.Windows.Forms.Label infoLabel;
        private System.Windows.Forms.TextBox IpAddressText;
        private System.Windows.Forms.Label titolo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox PortaText;
    }
}