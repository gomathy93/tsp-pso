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
					   float OMEGA, int REHOPE, int NOCHANGE) {
	srand(static_cast<unsigned int>(time(NULL)));
	Graph g(graph);
	if(g.V() == 0) {
		AlgoResults res = { -1, 0, 0.0f, 0.0f, 0.0f };
		return res;
	}

	PSO::TspSwarm swarm(&g, g.V(), C1, C2, OMEGA, REHOPE, NOCHANGE);
	SA::SimulatedAnnealing sa(&g);
	RS::RandomSearch rs(&g, NOCHANGE);

	swarm.compute();
	sa.compute();
	rs.compute();

	AlgoResults res = { 1, g.V(), swarm.getBestCost(), sa.getBestCost(), rs.getBestCost() };
	return res;
}

#ifdef AS_DLL
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	return 1;
}
#else
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout.precision(5);

	//params_test();
	same_graphs_test();
	
	system("PAUSE");
	return 0;
}
#endif
