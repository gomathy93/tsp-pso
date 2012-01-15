using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Globalization;
using System.Windows.Forms;

namespace PSO_GUI
{
    public class AlgoThread
    {
        public struct AlgoResults
        {
            public int result;
            public int verticesCount;
            public float psoCost;
            public float saCost;
            public float rsCost;
        };

        [DllImport("PSO.dll", CharSet = CharSet.Ansi)]
        public static extern AlgoResults solveGraph(
            string graph, float C1, float C2, float OMEGA, 
            Int32 REHOPE, Int32 NOCHANGE, Int32 particles);

        public struct IterCost
        {
            public IterCost(int iter, float cost)
            {
                this.iter = iter;
                this.cost = cost;
            }
            public int iter;
            public float cost;
        };

        // parametry
        string filename;
        float C1;
        float C2;
        float OMEGA;
        int REHOPE;
        int NOCHANGE;
        int particles;
        // wyniki
        public List<IterCost> psoRes = new List<IterCost>();
        public List<IterCost> saRes = new List<IterCost>();
        public List<IterCost> rsRes = new List<IterCost>();
        public int maxIter = 0;
        public float maxVal = 0.0f;
        public AlgoResults res;
        Form1 mainForm;

        public AlgoThread(Form1 mainForm, string filename,
            float C1, float C2, float OMEGA,
            int REHOPE, int NOCHANGE, int particles)
        {
            this.mainForm = mainForm;
            this.filename = filename;
            this.C1 = C1;
            this.C2 = C2;
            this.OMEGA = OMEGA;
            this.REHOPE = REHOPE;
            this.NOCHANGE = NOCHANGE;
            this.particles = particles;
        }

        public void Run()
        {
            maxIter = 0;
            maxVal = 0.0f;
            psoRes.Clear();
            saRes.Clear();
            rsRes.Clear();

            try
            {
                res = solveGraph(filename, C1, C2, OMEGA, REHOPE, NOCHANGE, particles);
                if (res.result == 1)
                {
                    CultureInfo ci = (CultureInfo)CultureInfo.CurrentCulture.Clone();
                    ci.NumberFormat.CurrencyDecimalSeparator = ".";
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
                        for (int i = 0; i < lines.Length - 3; i += 2)
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
                        for (int i = 0; i < lines.Length - 3; i += 2)
                        {
                            int iter = int.Parse(lines[i]);
                            float cost = float.Parse(lines[i + 1], NumberStyles.Any, ci);
                            rsRes.Add(new IterCost(iter, cost));
                            if (iter > maxIter) maxIter = iter;
                            if (cost > maxVal) maxVal = cost;
                        }
                    }
                    mainForm.OnAlgoEnd(this);
                }
                else if (res.result == 0)
                    throw new Exception("Błąd ładowania funkcji solveGraph z pliku PSO.dll");
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message);
            }
        }
    }
}
