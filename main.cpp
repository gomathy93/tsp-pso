#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
#include "RandomSearch.h"
#include "SimulatedAnnealing.h"

#include "Velocity.h"
#include "Position.h"

using namespace WMH;

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::cout.precision(5);

	// nieuzywane moje
	std::vector<PSO::PointSwap> swaps;
	swaps.push_back(PSO::PointSwap(1, 2));
	swaps.push_back(PSO::PointSwap(3, 4));
	swaps.push_back(PSO::PointSwap(5, 6));
	PSO::Velocity v(swaps);
	PSO::Velocity neg_v = ~v;
	PSO::Velocity sum_v = v + neg_v; // powinno byc puste
	PSO::Velocity mul02 = v * 0.2f; // mnozenie przez float
	PSO::Velocity mul22 = v * 2.2f;
	PSO::Velocity mul10 = v * 1.0f;
	PSO::Velocity mul_02 = v * -0.2f;
	PSO::Velocity sum = mul02 + mul22;

	const Graph g6(6, 10.0f);
	PSO::Position pos1(&g6);
	PSO::Position pos2(&g6);
	PSO::Velocity pos_diff1 = pos1 - pos2;
	PSO::Velocity pos_diff2 = pos2 - pos1;
	PSO::Position pos3 = pos1 + pos_diff1;
	PSO::Position pos4 = pos2 + pos_diff2;
	// koniec nieuzywane
	
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
