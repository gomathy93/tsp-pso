using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Globalization;
using System.Threading;

namespace PSO_GUI
{
    public delegate void OnAlgoEndHandler(AlgoThread algo);

    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Thread thread = null;
        AlgoThread algo = null;

        private void startButton_Click(object sender, EventArgs e)
        {
            if (thread != null && thread.IsAlive) return; // 1 watek na raz

            CultureInfo ci = (CultureInfo)CultureInfo.CurrentCulture.Clone();
            ci.NumberFormat.CurrencyDecimalSeparator = ".";
            try
            {
                float C1 = float.Parse(c1Val.Text, NumberStyles.Any, ci);
                float C2 = float.Parse(c1Val.Text, NumberStyles.Any, ci);
                float OMEGA = float.Parse(c1Val.Text, NumberStyles.Any, ci);
                Int32 REHOPE = Int32.Parse(rehopeVal.Text);
                Int32 NOCHANGE = Int32.Parse(noChangeVal.Text);
                Int32 particles = Int32.Parse(particlesVal.Text);
                if (openFile.ShowDialog() == DialogResult.OK)
                {
                    algo = new AlgoThread(this, openFile.FileName, 
                        C1, C2, OMEGA, REHOPE, NOCHANGE, particles);
                    thread = new Thread(algo.Run);
                    thread.Start();
                    startButton.Text = "Obliczanie ...";
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show("Błąd: " + exc.Message);
            }
        }

        public void OnAlgoEndHandler(AlgoThread algo)
        {
            vercicesCountLabel.Text = algo.res.verticesCount.ToString();
            psoCostLabel.Text = algo.res.psoCost.ToString();
            saCostLabel.Text = algo.res.saCost.ToString();
            randomCostLabel.Text = algo.res.rsCost.ToString();
            this.maxX = algo.maxIter;
            this.maxY = algo.maxVal;
            this.psoRes = algo.psoRes;
            this.rsRes = algo.rsRes;
            this.saRes = algo.saRes;
            startButton.Text = "Załaduj i rozpocznij";
            panel1.Invalidate();
        }

        public void OnAlgoEnd(AlgoThread algo)
        {
            if (this.InvokeRequired)
                this.Invoke(new OnAlgoEndHandler(OnAlgoEnd), algo);
            else
            {
                vercicesCountLabel.Text = algo.res.verticesCount.ToString();
                psoCostLabel.Text = algo.res.psoCost.ToString();
                saCostLabel.Text = algo.res.saCost.ToString();
                randomCostLabel.Text = algo.res.rsCost.ToString();
                this.maxX = algo.maxIter;
                this.maxY = algo.maxVal;
                this.psoRes = algo.psoRes;
                this.rsRes = algo.rsRes;
                this.saRes = algo.saRes;
                startButton.Text = "Załaduj i rozpocznij";
                panel1.Invalidate();
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            MessageBox.Show(
                "v(t+1) = OMEGA*v(t) + C1*(best-p(t)) + C2*(bestGlobal-p(t))\r\n" +
                "p(t+1) = p(t) + v(t+1)\r\n" +
                "Oznaczenia:\r\n" +
                "v(t) - prędkość cząstki w chwili t\r\n" +
                "p(t) - pozycja cząstki w chwili t\r\n" +
                "best - najlepsze rozwiązanie znalezione przez cząstkę\r\n" +
                "bestGlobal - najlepsze dotychczas znalezione rozwiązanie\r\n" +
                "C1, C2 - stałe przyspieszenia\r\n" + 
                "OMEGA - stała bezwładności");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Maksymalna ilość iteracji nie polepszających " +
                "najlepszego rozwiązania znalezionego przez cząstkę, po których " +
                "pozycja i prędkość cząstki zostanie ponownie zainicjowana");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Maksymalna ilość iteracji głównej pętli " +
                "algorytmu, nie polepszająca najlepszego znalezionego " + 
                "rozwiązania - warunek stopu");
        }

        Pen thickLine = new Pen(new SolidBrush(Color.Black), 2.0f);
        Pen thinLine = new Pen(new SolidBrush(Color.Black));
        Pen redLine = new Pen(new SolidBrush(Color.Red));
        Pen greenLine = new Pen(new SolidBrush(Color.Green));
        Pen blueLine = new Pen(new SolidBrush(Color.Blue));
        Brush blackBrush = new SolidBrush(Color.Black);
        Brush redBrush = new SolidBrush(Color.Red);
        Brush greenBrush = new SolidBrush(Color.Green);
        Brush blueBrush = new SolidBrush(Color.Blue);
        public List<PSO_GUI.AlgoThread.IterCost> psoRes = new List<PSO_GUI.AlgoThread.IterCost>();
        public List<PSO_GUI.AlgoThread.IterCost> saRes = new List<PSO_GUI.AlgoThread.IterCost>();
        public List<PSO_GUI.AlgoThread.IterCost> rsRes = new List<PSO_GUI.AlgoThread.IterCost>();
        public int maxX = 0;
        public float maxY = 0.0f;

        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {
            // margines wszedzie 10
            e.Graphics.Clear(panel1.BackColor);
            // osie
            e.Graphics.DrawLine(thickLine, 10, 20, 10, panel1.Height - 10); // pionowa
            e.Graphics.DrawLine(thickLine, 9, panel1.Height - 10, panel1.Width - 10, panel1.Height - 10); // pozioma
            e.Graphics.DrawLine(thickLine, 10, 20, 5, 25); // strzalka gora
            e.Graphics.DrawLine(thickLine, 10, 20, 15, 25);
            e.Graphics.DrawLine(thickLine, panel1.Width - 10, panel1.Height - 10, panel1.Width - 15, panel1.Height - 15); // strzalka prawo
            e.Graphics.DrawLine(thickLine, panel1.Width - 10, panel1.Height - 10, panel1.Width - 15, panel1.Height - 5);
            // skala 
            e.Graphics.DrawLine(thinLine, panel1.Width - 30, panel1.Height - 5, panel1.Width - 30, panel1.Height - 15);
            e.Graphics.DrawLine(thinLine, 5, 40, 15, 40);
            e.Graphics.DrawString("Koszt", Form1.DefaultFont, blackBrush, new PointF(0, 0));
            e.Graphics.DrawString("Iteracje", Form1.DefaultFont, blackBrush, new PointF(panel1.Width - 50, panel1.Height - 30));

            if (maxX > 0 && maxY > 0.0f)
            {
                int totalX = panel1.Width - 60;
                int totalY = panel1.Height - 50;
                for (int i = 0; i < psoRes.Count - 1; i++)
                {
                    e.Graphics.DrawLine(redLine, 
                        (psoRes[i].iter / (float)maxX) * totalX + 10,
                        ((maxY - psoRes[i].cost) / maxY) * totalY + 40,
                        (psoRes[i+1].iter / (float)maxX) * totalX + 10,
                        ((maxY - psoRes[i+1].cost) / maxY) * totalY + 40);
                }
                for (int i = 0; i < saRes.Count - 1; i++)
                {
                    e.Graphics.DrawLine(greenLine,
                        (saRes[i].iter / (float)maxX) * totalX + 10,
                        ((maxY - saRes[i].cost) / maxY) * totalY + 40,
                        (saRes[i + 1].iter / (float)maxX) * totalX + 10,
                        ((maxY - saRes[i + 1].cost) / maxY) * totalY + 40);
                }
                for (int i = 0; i < rsRes.Count - 1; i++)
                {
                    e.Graphics.DrawLine(blueLine,
                        (rsRes[i].iter / (float)maxX) * totalX + 10,
                        ((maxY - rsRes[i].cost) / maxY) * totalY + 40,
                        (rsRes[i + 1].iter / (float)maxX) * totalX + 10,
                        ((maxY - rsRes[i + 1].cost) / maxY) * totalY + 40);
                }
            }
        }

        private void panel1_Resize(object sender, EventArgs e)
        {
            panel1.Invalidate();
        }
    }
}
