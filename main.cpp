#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
#include "RandomSearch.h"
#include "SimulatedAnnealing.h"

using namespace WMH;

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout.precision(5);

	Graph g(50, 10.0f);
	//std::cout << g << std::endl;
	
	std::vector<iAlgo*>	algos;
	algos.push_back(new PSO::TspSwarm(&g, 50));
	algos.push_back(new RS::RandomSearch(&g));
	algos.push_back(new SA::SimulatedAnnealing(&g));
	std::cout << "Algorithm\t\tBest cost\tComputation time" << std::endl;
	for(unsigned int i=0; i<algos.size(); i++)
	{
		algos[i]->compute();
		std::cout << algos[i]->getAlgorithmName() << '\t' << algos[i]->getBestCost() << 
			"\t\t" << algos[i]->getComputationTime() << "ms" << std::endl;
		delete algos[i];
	}
	system("PAUSE");
	return 0;
}
