#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
#include "RandomSearch.h"
#include "SimulatedAnnealing.h"

using namespace WMH;

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout.precision(5);

	std::vector<Graph> graphs;
	graphs.push_back(Graph(5, 10.0f));
	graphs.push_back(Graph(50, 10.0f));
	graphs.push_back(Graph(500, 10.0f));
	graphs.push_back(Graph(1000, 10.0f));
	
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
