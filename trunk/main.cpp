#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
#include "RandomSearch.h"
#include "SimulatedAnnealing.h"

#include "Velocity.h"
#include "Position.h"

#include "tests.h"

using namespace WMH;

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout.precision(5);

	params_test();
	
	std::vector<Graph> graphs;
	graphs.push_back(Graph("art-graph.txt"));
	graphs.push_back(Graph("25-1.txt"));
	graphs.push_back(Graph("25-2.txt"));
	graphs.push_back(Graph("25-3.txt"));

	std::vector<iAlgo*>	algos;
	for(unsigned int i=0; i<graphs.size(); i++) {
		algos.push_back(new PSO::TspSwarm(&graphs[i], graphs[i].V()));
		algos.push_back(new RS::RandomSearch(&graphs[i]));
		algos.push_back(new SA::SimulatedAnnealing(&graphs[i]));
	}

	std::cout << "Algorithm\t\tBest cost\tComputation time\tVertices" << std::endl;
	for(unsigned int i=0; i<algos.size(); i++) {
		algos[i]->compute();
		std::cout << algos[i]->getAlgorithmName() << '\t' << algos[i]->getBestCost() << 
			"\t\t" << algos[i]->getComputationTime() << "ms\t\t\t" << algos[i]->getGraph()->V() << std::endl;
		delete algos[i];
	}
	system("PAUSE");
	return 0;
}
