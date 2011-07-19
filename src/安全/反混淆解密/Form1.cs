using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace 反混淆
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        private void m_btnDecode_Click(object sender, EventArgs e)
        {
            // 载入程序集，test.exe 为被混淆的程序集文件名。
            Assembly asm = Assembly.LoadFrom(m_edtTarget.Text);

            // 获取XenoCode插入的解密类型(包含其namespace)，对应上面字符串前面的类名，每次混淆结果可能都不同。
            Type type = asm.GetType(m_edtClassName.Text+".x1110bdd110cdcea4");

            // 字符串参数和解密参数
            object[] parameters = { m_edtEncoded.Text, Int32.Parse(m_edtEncodeLen.Text, System.Globalization.NumberStyles.HexNumber) };
            Type[] paramTypes = new Type[parameters.Length];
            for (int i = 0; i < parameters.Length; i++)
                paramTypes[i] = parameters[i].GetType();

            // 调用解密方法
            BindingFlags flags = BindingFlags.Public | BindingFlags.Static;
            MethodInfo method = type.GetMethod(m_edtMethodName.Text, flags, null, paramTypes, null);
            object result = method.Invoke(null, parameters);

            // 显示解密结果
            m_edtDecoded.Text = result.ToString();
        }

        private void m_btnSelTarget_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            //dlg.InitialDirectory = "d:\\";
            dlg.Filter = "exe file (*.exe)|*.exe|dll file (*.dll)|*.dll|All files (*.*)|*.*";
            dlg.FilterIndex = 1;
            dlg.RestoreDirectory = true;
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                m_edtTarget.Text = dlg.FileName;
            }
//             else
//             {
//                 m_edtTarget.Text = "";
//             } 
        }
    }
}