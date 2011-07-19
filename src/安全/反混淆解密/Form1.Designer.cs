namespace 反混淆
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.m_edtTarget = new System.Windows.Forms.TextBox();
            this.m_btnSelTarget = new System.Windows.Forms.Button();
            this.m_edtClassName = new System.Windows.Forms.TextBox();
            this.m_btnDecode = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.m_edtMethodName = new System.Windows.Forms.TextBox();
            this.m_edtEncoded = new System.Windows.Forms.TextBox();
            this.m_edtDecoded = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.m_edtEncodeLen = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // m_edtTarget
            // 
            this.m_edtTarget.Location = new System.Drawing.Point(75, 12);
            this.m_edtTarget.Name = "m_edtTarget";
            this.m_edtTarget.Size = new System.Drawing.Size(383, 21);
            this.m_edtTarget.TabIndex = 0;
            // 
            // m_btnSelTarget
            // 
            this.m_btnSelTarget.Location = new System.Drawing.Point(464, 12);
            this.m_btnSelTarget.Name = "m_btnSelTarget";
            this.m_btnSelTarget.Size = new System.Drawing.Size(31, 23);
            this.m_btnSelTarget.TabIndex = 1;
            this.m_btnSelTarget.Text = "...";
            this.m_btnSelTarget.UseVisualStyleBackColor = true;
            this.m_btnSelTarget.Click += new System.EventHandler(this.m_btnSelTarget_Click);
            // 
            // m_edtClassName
            // 
            this.m_edtClassName.Location = new System.Drawing.Point(75, 52);
            this.m_edtClassName.Name = "m_edtClassName";
            this.m_edtClassName.Size = new System.Drawing.Size(129, 21);
            this.m_edtClassName.TabIndex = 2;
            this.m_edtClassName.Text = "xb9d8bb5e6df032aa";
            // 
            // m_btnDecode
            // 
            this.m_btnDecode.Location = new System.Drawing.Point(222, 165);
            this.m_btnDecode.Name = "m_btnDecode";
            this.m_btnDecode.Size = new System.Drawing.Size(75, 23);
            this.m_btnDecode.TabIndex = 3;
            this.m_btnDecode.Text = "解密";
            this.m_btnDecode.UseVisualStyleBackColor = true;
            this.m_btnDecode.Click += new System.EventHandler(this.m_btnDecode_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(34, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "目标:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(34, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "类名:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(220, 58);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "方法名:";
            // 
            // m_edtMethodName
            // 
            this.m_edtMethodName.Location = new System.Drawing.Point(273, 52);
            this.m_edtMethodName.Name = "m_edtMethodName";
            this.m_edtMethodName.Size = new System.Drawing.Size(119, 21);
            this.m_edtMethodName.TabIndex = 7;
            this.m_edtMethodName.Text = "_xaacba899487bce8c";
            // 
            // m_edtEncoded
            // 
            this.m_edtEncoded.Location = new System.Drawing.Point(75, 94);
            this.m_edtEncoded.Name = "m_edtEncoded";
            this.m_edtEncoded.Size = new System.Drawing.Size(317, 21);
            this.m_edtEncoded.TabIndex = 8;
            // 
            // m_edtDecoded
            // 
            this.m_edtDecoded.Location = new System.Drawing.Point(75, 133);
            this.m_edtDecoded.Name = "m_edtDecoded";
            this.m_edtDecoded.Size = new System.Drawing.Size(420, 21);
            this.m_edtDecoded.TabIndex = 9;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(34, 97);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 12);
            this.label4.TabIndex = 10;
            this.label4.Text = "密文:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(34, 136);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 12);
            this.label5.TabIndex = 11;
            this.label5.Text = "明文:";
            // 
            // m_edtEncodeLen
            // 
            this.m_edtEncodeLen.Location = new System.Drawing.Point(398, 94);
            this.m_edtEncodeLen.Name = "m_edtEncodeLen";
            this.m_edtEncodeLen.Size = new System.Drawing.Size(97, 21);
            this.m_edtEncodeLen.TabIndex = 13;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(524, 200);
            this.Controls.Add(this.m_edtEncodeLen);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.m_edtDecoded);
            this.Controls.Add(this.m_edtEncoded);
            this.Controls.Add(this.m_edtMethodName);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.m_btnDecode);
            this.Controls.Add(this.m_edtClassName);
            this.Controls.Add(this.m_btnSelTarget);
            this.Controls.Add(this.m_edtTarget);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox m_edtTarget;
        private System.Windows.Forms.Button m_btnSelTarget;
        private System.Windows.Forms.TextBox m_edtClassName;
        private System.Windows.Forms.Button m_btnDecode;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox m_edtMethodName;
        private System.Windows.Forms.TextBox m_edtEncoded;
        private System.Windows.Forms.TextBox m_edtDecoded;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox m_edtEncodeLen;
    }
}

