#pragma once

#include "Algo.h"
#include "Graph.h"

namespace WMH {
	namespace RS {
		/** Losowe przeszukiwanie przestrzeni rozwiazan (TSP) */
		class RandomSearch : public iAlgo
		{
			/** Maksymalna liczba iteracji ktora nie poprawia globalnego wyniku */
			int		NOCHANGE_MAX;
			/** Graf dla ktorego przeprowadzane sa obliczenia */
			Graph*	g;
			/** Najlepsze znalezione rozwiazanie */
			float	bestCost;
			/** Cykl dla najlepszego rozwiazania */
			std::vector<int>	best;
			/** Czas obliczen w ms */
			DWORD computationTime;
			/** Liczba iteracji ktora nie poprawila wyniku */
			int		noChange;
		public:

			/** Inicjuje przeszukiwanie rozwiazan dla TSP dla grafu g */
			RandomSearch(Graph* g, int maxNoChange = 1000)
			{
				NOCHANGE_MAX = maxNoChange;
				this->g = g;
				bestCost = MAX_FLOAT;
				computationTime = 0;
				noChange = 0;
			}

			/** Rozpoczyna obliczenia */
			void compute()
			{
				computationTime = GetTickCount();
				noChange = 0;
				
				// inicjacja
				std::vector<int> solution(g->V());
				for(int i=0; i<g->V(); i++)
					solution[i] = i;
				bestCost = g->hamiltonLength(solution);
				best = solution;

				while(noChange < NOCHANGE_MAX)
				{
					noChange++;
					std::swap(solution[rand()%g->V()], solution[rand()%g->V()]);
					float cost = g->hamiltonLength(solution);
					if(cost < bestCost)
					{
						//std::cout << "Better cost found: " << cost << " < " << bestCost << std::endl;
						bestCost = cost;
						best = solution;
						noChange = 0;
					}
				}
				computationTime = GetTickCount() - computationTime;
			}

			/** Zwraca koszt najlepszego rozwiazania */
			inline float getBestCost() const
			{
				return bestCost;
			}

			/** Zwraca najlepsze rozwiazanie */
			inline std::vector<int> getBestSolution() const
			{
				return best;
			}

			/** Zwraca nazwe algorytmu */
			inline const char* getAlgorithmName() const
			{
				return "RandomSearch\t";
			}

			/** Zwraca czas obliczen w ms */
			inline DWORD getComputationTime() const
			{
				return computationTime;
			}
		};
	}; // namespace RS
}; // namespace WMH
