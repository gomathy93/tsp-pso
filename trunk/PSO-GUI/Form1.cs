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
            try
            {
                float C1 = float.Parse(c1Val.Text);
                float C2 = float.Parse(c1Val.Text);
                float OMEGA = float.Parse(c1Val.Text);
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
    }
}
