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

	PSO::Position pos1;
	PSO::Position pos2;
	pos1.randomize(10);
	pos2.randomize(10);
	//pos1[0] = 1; pos1[1] = 0; pos1[2] = 2; pos1[3] = 4; pos1[4] = 3;
	//pos2[0] = 2; pos2[1] = 4; pos2[2] = 0; pos2[3] = 3; pos2[4] = 1;
	PSO::Velocity v1 = pos1 - pos2;
	PSO::Velocity v2 = pos2 - pos1;

	std::cout << "vel1 " <<std::endl;
	for(size_t x =0; x < v1.size();x++)
		std::cout<< "(" << v1[x].i << ","<<v1[x].j<<") ";
	std::cout<<std::endl;
	
	std::cout << "val2 " <<std::endl;
	for(size_t xx =0; xx < v2.size();xx++)
		std::cout<< "(" << v2[xx].i << ","<<v2[xx].j<<") ";
	std::cout<<std::endl;
	
		
	std::cout << "p1: " << pos1;
	std::cout << "p2: " << pos2;
	PSO::Position pos11 = pos2; pos11 += v1;
	PSO::Position pos22 = pos1; pos22 += v2;
	std::cout << "p1: " << pos11;
	std::cout << "p2: " << pos22;

	// koniec nieuzywane
	std::ofstream fout("50-1.txt");
	fout << Graph(50, 10.0f) << std::endl;
	fout.close();
	
	std::vector<Graph> graphs;
	graphs.push_back(Graph("25-1.txt"));
	graphs.push_back(Graph("25-2.txt"));
	graphs.push_back(Graph("25-3.txt"));
	//graphs.push_back(Graph("50-1.txt"));
	
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
