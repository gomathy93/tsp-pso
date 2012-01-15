#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
#include "RandomSearch.h"
#include "SimulatedAnnealing.h"

#include "Velocity.h"
#include "Position.h"

#include "tests.h"

using namespace WMH;

AlgoResults solveGraph(const char* graph, float C1, float C2,
					   float OMEGA, int REHOPE, int NOCHANGE, int particles) {
	srand(static_cast<unsigned int>(time(NULL)));
	Graph g(graph);
	if(g.V() == 0) {
		AlgoResults res = { -1, 0, 0.0f, 0.0f, 0.0f };
		return res;
	}

	PSO::TspSwarm swarm(&g, particles, C1, C2, OMEGA, REHOPE, NOCHANGE);
	SA::SimulatedAnnealing sa(&g);
	RS::RandomSearch rs(&g, NOCHANGE);

	swarm.compute(true);
	sa.compute(true);
	rs.compute(true);

	std::ofstream fout;
	std::vector<IterCost> results;

	fout.open("pso.txt");
	results = swarm.getCostTable();
	for(size_t i=0; i<results.size()-1; i++)
		fout << results[i].iter << std::endl << results[i].cost << std::endl;
	fout << results[results.size()-1].iter << std::endl << results[results.size()-1].cost;
	fout.close();

	fout.open("sa.txt");
	results = sa.getCostTable();
	for(size_t i=0; i<results.size()-1; i++)
		fout << results[i].iter << std::endl << results[i].cost << std::endl;
	fout << results[results.size()-1].iter << std::endl << results[results.size()-1].cost;
	fout.close();

	fout.open("rs.txt");
	results = rs.getCostTable();
	for(size_t i=0; i<results.size()-1; i++)
		fout << results[i].iter << std::endl << results[i].cost << std::endl;
	fout << results[results.size()-1].iter << std::endl << results[results.size()-1].cost;
	fout.close();

	AlgoResults ret = { 1, g.V(), swarm.getBestCost(), sa.getBestCost(), rs.getBestCost() };
	return ret;
}

#ifdef AS_DLL
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	return 1;
}
#else
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout.precision(5);

	generate_and_save("50-1.txt", 50);
	generate_and_save("100-1.txt", 100);

	//params_test();
	same_graphs_test();
	
	system("PAUSE");
	return 0;
}
#endif
