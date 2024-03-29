#pragma once

#include "Algo.h"
#include "Graph.h"

namespace WMH {
	namespace RS {
		/** Losowe przeszukiwanie przestrzeni rozwiazan (TSP) */
		class RandomSearch : public iAlgo {
			/** Maksymalna liczba iteracji ktora nie poprawia globalnego wyniku */
			int		NOCHANGE_MAX;
			/** Graf dla ktorego przeprowadzane sa obliczenia */
			const Graph*	g;
			/** Najlepsze znalezione rozwiazanie */
			float	bestCost;
			/** Cykl dla najlepszego rozwiazania */
			std::vector<int>	best;
			/** Czas obliczen w ms */
			DWORD computationTime;
			/** Ogolna liczba iteracji */
			int		iter;
			/** Liczba iteracji ktora nie poprawila wyniku */
			int		noChange;
			/** Na potrzeby rysowania wykresu, wyniki w kolejnych iteracjach */
			std::vector<IterCost>	iterResults;
		public:
			/** Inicjuje przeszukiwanie rozwiazan dla TSP dla grafu g */
			RandomSearch(const Graph* g, int maxNoChange = 1000) {
				NOCHANGE_MAX = maxNoChange;
				this->g = g;
				bestCost = MAX_FLOAT;
				computationTime = 0;
				noChange = 0;
				iter = 0;
			}

			/** Rozpoczyna obliczenia */
			void compute(bool saveResults) {
				computationTime = GetTickCount();
				
				// inicjacja
				std::vector<int> solution(g->V());
				for(int i=0; i<g->V(); i++)
					solution[i] = i;
				bestCost = g->hamiltonLength(solution);
				best = solution;

				noChange = 0;
				iter = 0;
				iterResults.clear();
				if(saveResults)
					iterResults.push_back(IterCost(iter, bestCost));

				while(noChange < NOCHANGE_MAX) {
					noChange++;
					iter++;
					std::swap(solution[rand()%g->V()], solution[rand()%g->V()]);
					float cost = g->hamiltonLength(solution);
					if(cost < bestCost) {
						bestCost = cost;
						best = solution;
						noChange = 0;

						if(saveResults)
							iterResults.push_back(IterCost(iter, bestCost));
					}
				}

				if(saveResults)
					iterResults.push_back(IterCost(iter, bestCost));
				computationTime = GetTickCount() - computationTime;
			}

			/** Zwraca koszt najlepszego rozwiazania */
			inline float getBestCost() const {
				return bestCost;
			}

			/** Zwraca najlepsze rozwiazanie */
			inline std::vector<int> getBestSolution() const	{
				return best;
			}

			/** Zwraca nazwe algorytmu */
			inline const char* getAlgorithmName() const {
				return "RandomSearch\t";
			}

			/** Zwraca tabele kosztow w poszczegolnych iteracjach */
			inline std::vector<IterCost> getCostTable() const {
				return iterResults;
			}

			/** Zwraca czas obliczen w ms */
			inline DWORD getComputationTime() const {
				return computationTime;
			}

			/** Zwraca graf na ktorym wykonywane sa obliczenia */
			virtual const Graph* getGraph() const {
				return g;
			}
		};
	}; // namespace RS
}; // namespace WMH
