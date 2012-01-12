#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
#include "SimulatedAnnealing.h"
#include "RandomSearch.h"
#include "utils.h"

namespace WMH {
	void graph_test() {
		Graph g("simple-graph.txt");
		
		// testy zliczania dlugosci cyklu
		std::vector<int> cycle1(g.V());
		cycle1[0] = 1; cycle1[1] = 0; cycle1[2] = 2; cycle1[3] = 3;
		assert(g.hamiltonLength(cycle1) == 12); // 2 + 2 + 5 + 3

		std::vector<int> cycle2(g.V());
		cycle2[0] = 2; cycle2[1] = 1; cycle2[2] = 3; cycle2[3] = 0; 
		assert(g.hamiltonLength(cycle2) == 9); // 1 + 2 + 4 + 2

		// testy konstruktorow kopiujacych i operatora przypisania
		Graph g2(g);
		assert(g2.hamiltonLength(cycle1) == 12);
		assert(g2.hamiltonLength(cycle2) == 9);

		Graph g3(10);
		g3 = g;
		assert(g3.hamiltonLength(cycle1) == 12);
		assert(g3.hamiltonLength(cycle2) == 9);
	}

	void operators_test() {
		/*std::vector<PSO::PointSwap> swaps;
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
		*/
	}

	void same_graphs_test(float C1, float C2, float OMEGA, int REHOPE) {
		std::vector<Graph> graphs;
		graphs.push_back(Graph("art-graph.txt"));
		graphs.push_back(Graph("25-1.txt"));
		graphs.push_back(Graph("25-2.txt"));
		graphs.push_back(Graph("25-3.txt"));

		std::vector<iAlgo*>	algos;
		for(unsigned int i=0; i<graphs.size(); i++) {
			algos.push_back(new PSO::TspSwarm(&graphs[i], graphs[i].V(), C1, C2, OMEGA, REHOPE));
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
	}

	void params_test() {
		float bestScore = MAX_FLOAT;
		float bestC1 = 0.0f;
		float bestC2 = 0.0f;
		float bestOMEGA = 0.0f;
		int bestREHOPE = 0;
		int bestIndex = -1;

		const int CHECKS = 20;
		const int MEAN = 20;
		// i sprawdzen losowych wartosci
		for(int i=0; i<CHECKS; i++) {
			std::cout << "Check " << i + 1 << " of " << CHECKS << std::endl;
			float C1 = randf(-1.9f, 1.9f);
			float C2 = randf(-1.9f, 1.9f);
			float OMEGA = randf(-1.9f, 1.9f);
			int REHOPE = ((rand() % 50) + 1) * 10;
			float avScore = 0.0f;
			float avScoreSA = 0.0f;
			float avScoreRS = 0.0f;
			// usrednienie dla j grafow
			for(int j=0; j<MEAN; j++) {
				Graph g(20, 1.0f, 10.0f);
				PSO::TspSwarm swarm(&g, g.V(), C1, C2, OMEGA, REHOPE);
				SA::SimulatedAnnealing sa(&g);
				RS::RandomSearch rnd(&g);
				swarm.compute();
				sa.compute();
				rnd.compute();
				avScore += swarm.getBestCost();
				avScoreSA += sa.getBestCost();
				avScoreRS += rnd.getBestCost();
			}
			avScore /= static_cast<float>(MEAN);
			avScoreSA /= static_cast<float>(MEAN);
			avScoreRS /= static_cast<float>(MEAN);
			std::cout << "Average score (" << MEAN << " checks): " << avScore << " (SA: " << avScoreSA 
				<< ", RS: " << avScoreRS << ")" << std::endl;
			if(avScore - avScoreSA < bestScore) {
				bestIndex = i + 1;
				bestScore = avScore - avScoreSA;
				bestC1 = C1;
				bestC2 = C2;
				bestOMEGA = OMEGA;
				bestREHOPE = REHOPE;
			}
		}
		std::cout << "Best index: " << bestIndex << std::endl;
		std::cout << "Best C1: " << bestC1 << std::endl;
		std::cout << "Best C2: " << bestC2 << std::endl;
		std::cout << "Best OMEGA: " << bestOMEGA << std::endl;
		std::cout << "Best REHOPE: " << bestREHOPE << std::endl;

		same_graphs_test(bestC1, bestC2, bestOMEGA, bestREHOPE);
	}
}
