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
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.chiudiToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.scegliCartellaDaControllareToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.comeUsareToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.problemiComuniToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.internetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sviluppatoreToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageList2 = new System.Windows.Forms.ImageList(this.components);
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuToolStripMenuItem,
            this.toolStripMenuItem1,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.menuStrip1.Size = new System.Drawing.Size(686, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menuStrip1_ItemClicked);
            // 
            // menuToolStripMenuItem
            // 
            this.menuToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.chiudiToolStripMenuItem,
            this.scegliCartellaDaControllareToolStripMenuItem});
            this.menuToolStripMenuItem.Name = "menuToolStripMenuItem";
            this.menuToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.menuToolStripMenuItem.Text = "menu";
            // 
            // chiudiToolStripMenuItem
            // 
            this.chiudiToolStripMenuItem.Name = "chiudiToolStripMenuItem";
            this.chiudiToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.chiudiToolStripMenuItem.Text = "Client";
            this.chiudiToolStripMenuItem.Click += new System.EventHandler(this.chiudiToolStripMenuItem_Click);
            // 
            // scegliCartellaDaControllareToolStripMenuItem
            // 
            this.scegliCartellaDaControllareToolStripMenuItem.Name = "scegliCartellaDaControllareToolStripMenuItem";
            this.scegliCartellaDaControllareToolStripMenuItem.Size = new System.Drawing.Size(222, 22);
            this.scegliCartellaDaControllareToolStripMenuItem.Text = "Scegli cartella da controllare";
            this.scegliCartellaDaControllareToolStripMenuItem.Click += new System.EventHandler(this.scegliCartellaDaControllareToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.comeUsareToolStripMenuItem,
            this.problemiComuniToolStripMenuItem,
            this.internetToolStripMenuItem});
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(46, 20);
            this.toolStripMenuItem1.Text = "aiuto";
            // 
            // comeUsareToolStripMenuItem
            // 
            this.comeUsareToolStripMenuItem.Name = "comeUsareToolStripMenuItem";
            this.comeUsareToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.comeUsareToolStripMenuItem.Text = "come Usare";
            // 
            // problemiComuniToolStripMenuItem
            // 
            this.problemiComuniToolStripMenuItem.Name = "problemiComuniToolStripMenuItem";
            this.problemiComuniToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.problemiComuniToolStripMenuItem.Text = "Problemi comuni";
            // 
            // internetToolStripMenuItem
            // 
            this.internetToolStripMenuItem.Name = "internetToolStripMenuItem";
            this.internetToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.internetToolStripMenuItem.Text = "Internet";
            this.internetToolStripMenuItem.Click += new System.EventHandler(this.internetToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sviluppatoreToolStripMenuItem});
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.aboutToolStripMenuItem.Text = "about";
            // 
            // sviluppatoreToolStripMenuItem
            // 
            this.sviluppatoreToolStripMenuItem.Name = "sviluppatoreToolStripMenuItem";
            this.sviluppatoreToolStripMenuItem.Size = new System.Drawing.Size(140, 22);
            this.sviluppatoreToolStripMenuItem.Text = "Sviluppatore";
            this.sviluppatoreToolStripMenuItem.Click += new System.EventHandler(this.sviluppatoreToolStripMenuItem_Click);
            // 
            // imageList2
            // 
            this.imageList2.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList2.ImageSize = new System.Drawing.Size(16, 16);
            this.imageList2.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoScroll = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(12, 27);
            this.flowLayoutPanel1.MinimumSize = new System.Drawing.Size(400, 300);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(400, 344);
            this.flowLayoutPanel1.TabIndex = 3;
            this.flowLayoutPanel1.DoubleClick += new System.EventHandler(this.flowLayoutPanel1_DoubleClick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.BackgroundImage = global::PrimaGUI.Properties.Resources.sfondoAzzurrino;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(686, 388);
            this.Controls.Add(this.flowLayoutPanel1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.TopMost = true;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem chiudiToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem comeUsareToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem problemiComuniToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sviluppatoreToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem internetToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem scegliCartellaDaControllareToolStripMenuItem;
        private System.Windows.Forms.ImageList imageList2;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
    }
}

