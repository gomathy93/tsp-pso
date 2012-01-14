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

namespace PSO_GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public struct AlgoResults
        {
            public int result;
            public int verticesCount;
            public float psoCost;
            public float saCost;
            public float rsCost;
        };

        [DllImport("PSO.dll", CharSet = CharSet.Ansi)]
        public static extern AlgoResults solveGraph(string graph, float C1, float C2, float OMEGA, Int32 REHOPE, Int32 NOCHANGE);

        private void startButton_Click(object sender, EventArgs e)
        {
            CultureInfo ci = (CultureInfo)CultureInfo.CurrentCulture.Clone();
            ci.NumberFormat.CurrencyDecimalSeparator = ".";

            maxIter = 0;
            maxVal = 0.0f;
            psoRes.Clear();
            saRes.Clear();
            rsRes.Clear();
            try
            {
                float C1 = float.Parse(c1Val.Text, NumberStyles.Any, ci);
                float C2 = float.Parse(c1Val.Text, NumberStyles.Any, ci);
                float OMEGA = float.Parse(c1Val.Text, NumberStyles.Any, ci);
                Int32 REHOPE = Int32.Parse(rehopeVal.Text);
                Int32 NOCHANGE = Int32.Parse(noChangeVal.Text);
                if (openFile.ShowDialog() == DialogResult.OK)
                {
                    AlgoResults res = solveGraph(openFile.FileName, 
                        C1, C2, OMEGA, REHOPE, NOCHANGE);
                    if (res.result == 1)
                    {
                        vercicesCountLabel.Text = res.verticesCount.ToString();
                        psoCostLabel.Text = res.psoCost.ToString();
                        saCostLabel.Text = res.saCost.ToString();
                        randomCostLabel.Text = res.rsCost.ToString();

                        if (File.Exists("pso.txt"))
                        {
                            string[] lines = File.ReadAllLines("pso.txt");
                            for (int i = 0; i < lines.Length - 3; i += 2)
                            {
                                int iter = int.Parse(lines[i]);
                                float cost = float.Parse(lines[i + 1], NumberStyles.Any, ci);
                                psoRes.Add(new IterCost(iter, cost));
                                if (iter > maxIter) maxIter = iter;
                                if (cost > maxVal) maxVal = cost;
                            }
                        }
                        if (File.Exists("sa.txt"))
                        {
                            string[] lines = File.ReadAllLines("sa.txt");
                            for (int i = 0; i < lines.Length - 3; i+=2)
                            {
                                int iter = int.Parse(lines[i]);
                                float cost = float.Parse(lines[i + 1], NumberStyles.Any, ci);
                                saRes.Add(new IterCost(iter, cost));
                                if (iter > maxIter) maxIter = iter;
                                if (cost > maxVal) maxVal = cost;
                            }
                        }
                        if (File.Exists("rs.txt"))
                        {
                            string[] lines = File.ReadAllLines("rs.txt");
                            for (int i = 0; i < lines.Length - 3; i+=2)
                            {
                                int iter = int.Parse(lines[i]);
                                float cost = float.Parse(lines[i + 1], NumberStyles.Any, ci);
                                rsRes.Add(new IterCost(iter, cost));
                                if (iter > maxIter) maxIter = iter;
                                if (cost > maxVal) maxVal = cost;
                            }
                        }
                        panel1.Invalidate();
                    }
                    else if(res.result == 0)
                        throw new Exception("Błąd ładowania funkcji solveGraph z pliku PSO.dll");
                }
            }
            catch (Exception exc)
            {
                MessageBox.Show("Błąd: " + exc.Message);
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

        struct IterCost
        {
            public IterCost(int iter, float cost)
            {
                this.iter = iter;
                this.cost = cost;
            }
            public int iter;
            public float cost;
        };

        List<IterCost> psoRes = new List<IterCost>();
        List<IterCost> saRes = new List<IterCost>();
        List<IterCost> rsRes = new List<IterCost>();
        int maxIter = 0;
        float maxVal = 0.0f;

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

            if (maxIter > 0 && maxVal > 0.0f)
            {
                int totalX = panel1.Width - 60;
                int totalY = panel1.Height - 50;
                for (int i = 0; i < psoRes.Count - 1; i++)
                {
                    e.Graphics.DrawLine(redLine, 
                        (psoRes[i].iter / (float)maxIter) * totalX + 10,
                        ((maxVal - psoRes[i].cost) / maxVal) * totalY + 40,
                        (psoRes[i+1].iter / (float)maxIter) * totalX + 10,
                        ((maxVal - psoRes[i+1].cost) / maxVal) * totalY + 40);
                }
                for (int i = 0; i < saRes.Count - 1; i++)
                {
                    e.Graphics.DrawLine(greenLine,
                        (saRes[i].iter / (float)maxIter) * totalX + 10,
                        ((maxVal - saRes[i].cost) / maxVal) * totalY + 40,
                        (saRes[i + 1].iter / (float)maxIter) * totalX + 10,
                        ((maxVal - saRes[i + 1].cost) / maxVal) * totalY + 40);
                }
                for (int i = 0; i < rsRes.Count - 1; i++)
                {
                    e.Graphics.DrawLine(blueLine,
                        (rsRes[i].iter / (float)maxIter) * totalX + 10,
                        ((maxVal - rsRes[i].cost) / maxVal) * totalY + 40,
                        (rsRes[i + 1].iter / (float)maxIter) * totalX + 10,
                        ((maxVal - rsRes[i + 1].cost) / maxVal) * totalY + 40);
                }
            }
        }

        private void panel1_Resize(object sender, EventArgs e)
        {
            panel1.Invalidate();
        }
    }
}
