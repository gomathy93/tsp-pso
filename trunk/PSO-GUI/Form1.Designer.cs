namespace PSO_GUI
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
            this.openFile = new System.Windows.Forms.OpenFileDialog();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.psoCostLabel = new System.Windows.Forms.Label();
            this.randomCostLabel = new System.Windows.Forms.Label();
            this.saCostLabel = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.vercicesCountLabel = new System.Windows.Forms.Label();
            this.startButton = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.particlesVal = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.button6 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.noChangeVal = new System.Windows.Forms.TextBox();
            this.rehopeVal = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.omegaVal = new System.Windows.Forms.TextBox();
            this.c2Val = new System.Windows.Forms.TextBox();
            this.c1Val = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFile
            // 
            this.openFile.Filter = "Pliki z grafami|*.txt";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.groupBox3);
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.vercicesCountLabel);
            this.groupBox2.Controls.Add(this.startButton);
            this.groupBox2.Location = new System.Drawing.Point(453, 193);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(172, 186);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Obliczenia";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.psoCostLabel);
            this.groupBox3.Controls.Add(this.randomCostLabel);
            this.groupBox3.Controls.Add(this.saCostLabel);
            this.groupBox3.Controls.Add(this.label11);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Location = new System.Drawing.Point(6, 61);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(157, 117);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Wyniki";
            // 
            // psoCostLabel
            // 
            this.psoCostLabel.AutoSize = true;
            this.psoCostLabel.Location = new System.Drawing.Point(11, 29);
            this.psoCostLabel.Name = "psoCostLabel";
            this.psoCostLabel.Size = new System.Drawing.Size(10, 13);
            this.psoCostLabel.TabIndex = 14;
            this.psoCostLabel.Text = "-";
            this.psoCostLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // randomCostLabel
            // 
            this.randomCostLabel.AutoSize = true;
            this.randomCostLabel.Location = new System.Drawing.Point(11, 82);
            this.randomCostLabel.Name = "randomCostLabel";
            this.randomCostLabel.Size = new System.Drawing.Size(10, 13);
            this.randomCostLabel.TabIndex = 13;
            this.randomCostLabel.Text = "-";
            this.randomCostLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // saCostLabel
            // 
            this.saCostLabel.AutoSize = true;
            this.saCostLabel.Location = new System.Drawing.Point(11, 56);
            this.saCostLabel.Name = "saCostLabel";
            this.saCostLabel.Size = new System.Drawing.Size(10, 13);
            this.saCostLabel.TabIndex = 11;
            this.saCostLabel.Text = "-";
            this.saCostLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.ForeColor = System.Drawing.Color.Blue;
            this.label11.Location = new System.Drawing.Point(8, 69);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(89, 13);
            this.label11.TabIndex = 10;
            this.label11.Text = "Losowy przegląd:";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.ForeColor = System.Drawing.Color.Green;
            this.label10.Location = new System.Drawing.Point(8, 42);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(125, 13);
            this.label10.TabIndex = 9;
            this.label10.Text = "Symulowane wyżarzanie:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.ForeColor = System.Drawing.Color.Red;
            this.label6.Location = new System.Drawing.Point(8, 16);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(140, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "Particle Swarm Optimization:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 45);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(79, 13);
            this.label8.TabIndex = 4;
            this.label8.Text = "Wierzchołków:";
            // 
            // vercicesCountLabel
            // 
            this.vercicesCountLabel.AutoSize = true;
            this.vercicesCountLabel.Location = new System.Drawing.Point(91, 45);
            this.vercicesCountLabel.Name = "vercicesCountLabel";
            this.vercicesCountLabel.Size = new System.Drawing.Size(10, 13);
            this.vercicesCountLabel.TabIndex = 2;
            this.vercicesCountLabel.Text = "-";
            this.vercicesCountLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // startButton
            // 
            this.startButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.startButton.Location = new System.Drawing.Point(9, 19);
            this.startButton.Name = "startButton";
            this.startButton.Size = new System.Drawing.Size(154, 23);
            this.startButton.TabIndex = 0;
            this.startButton.Text = "Załaduj i rozpocznij";
            this.startButton.UseVisualStyleBackColor = true;
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.particlesVal);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.button6);
            this.groupBox1.Controls.Add(this.button5);
            this.groupBox1.Controls.Add(this.button4);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.noChangeVal);
            this.groupBox1.Controls.Add(this.rehopeVal);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.omegaVal);
            this.groupBox1.Controls.Add(this.c2Val);
            this.groupBox1.Controls.Add(this.c1Val);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(452, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(173, 173);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Parametry";
            // 
            // particlesVal
            // 
            this.particlesVal.Location = new System.Drawing.Point(80, 13);
            this.particlesVal.Name = "particlesVal";
            this.particlesVal.Size = new System.Drawing.Size(49, 20);
            this.particlesVal.TabIndex = 16;
            this.particlesVal.Text = "25";
            this.particlesVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(9, 16);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(65, 13);
            this.label7.TabIndex = 15;
            this.label7.Text = "Cząsteczek:";
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(134, 91);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(20, 20);
            this.button6.TabIndex = 14;
            this.button6.Text = "?";
            this.button6.UseVisualStyleBackColor = true;
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(134, 65);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(20, 20);
            this.button5.TabIndex = 13;
            this.button5.Text = "?";
            this.button5.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(134, 39);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(20, 20);
            this.button4.TabIndex = 12;
            this.button4.Text = "?";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(134, 145);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(20, 20);
            this.button2.TabIndex = 11;
            this.button2.Text = "?";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(134, 118);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(20, 20);
            this.button1.TabIndex = 10;
            this.button1.Text = "?";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(42, 148);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Stop:";
            // 
            // noChangeVal
            // 
            this.noChangeVal.Location = new System.Drawing.Point(80, 145);
            this.noChangeVal.Name = "noChangeVal";
            this.noChangeVal.Size = new System.Drawing.Size(49, 20);
            this.noChangeVal.TabIndex = 8;
            this.noChangeVal.Text = "1000";
            this.noChangeVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // rehopeVal
            // 
            this.rehopeVal.Location = new System.Drawing.Point(80, 118);
            this.rehopeVal.Name = "rehopeVal";
            this.rehopeVal.Size = new System.Drawing.Size(49, 20);
            this.rehopeVal.TabIndex = 7;
            this.rehopeVal.Text = "80";
            this.rehopeVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 122);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(55, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "REHOPE:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(26, 94);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(49, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "OMEGA:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(52, 68);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(23, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "C2:";
            // 
            // omegaVal
            // 
            this.omegaVal.Location = new System.Drawing.Point(80, 91);
            this.omegaVal.Name = "omegaVal";
            this.omegaVal.Size = new System.Drawing.Size(49, 20);
            this.omegaVal.TabIndex = 3;
            this.omegaVal.Text = "1.43";
            this.omegaVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // c2Val
            // 
            this.c2Val.Location = new System.Drawing.Point(80, 65);
            this.c2Val.Name = "c2Val";
            this.c2Val.Size = new System.Drawing.Size(49, 20);
            this.c2Val.TabIndex = 2;
            this.c2Val.Text = "-0.0782";
            this.c2Val.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // c1Val
            // 
            this.c1Val.Location = new System.Drawing.Point(80, 39);
            this.c1Val.Name = "c1Val";
            this.c1Val.Size = new System.Drawing.Size(49, 20);
            this.c1Val.TabIndex = 1;
            this.c1Val.Text = "0.45895";
            this.c1Val.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(52, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "C1:";
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(434, 367);
            this.panel1.TabIndex = 6;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.splitContainer1_Panel1_Paint);
            this.panel1.Resize += new System.EventHandler(this.panel1_Resize);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(637, 390);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Particle Swarm Optimization";
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFile;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label psoCostLabel;
        private System.Windows.Forms.Label randomCostLabel;
        private System.Windows.Forms.Label saCostLabel;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label vercicesCountLabel;
        private System.Windows.Forms.Button startButton;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox noChangeVal;
        private System.Windows.Forms.TextBox rehopeVal;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox omegaVal;
        private System.Windows.Forms.TextBox c2Val;
        private System.Windows.Forms.TextBox c1Val;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox particlesVal;
        private System.Windows.Forms.Label label7;
    }
}

