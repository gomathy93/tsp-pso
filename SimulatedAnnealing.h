#pragma once

#include "stdafx.h"
#include "utils.h"

namespace WMH {
	namespace SA {
		/** TSP metoda symulowanego wyzarzania */
		class SimulatedAnnealing : public iAlgo
		{
			/** Przetwarzany graf */
			Graph*				g;
			/** Aktualna temperatura */
			float				temp;
			/** Temperatura poczatkowa */
			float				START_TEMP;
			/** Temperatura przy ktorej algorytm sie zakonczy */
			float				STOP_TEMP;
			/** Mnoznik obnizania temperatury */
			float				COOLING_FACTOR;
			/** Maksymalna ilosc iteracji wewnetrznej petli (najwazniejszy paramet) */
			int					INNER_ITER_MAX;
			/** Iteracje w wewnetrznej petli */
			int					innerInter;
			/** Koszt poprzedniego rozwiazania */
			float				prevCost;
			/** Poprzednie rozwiazanie */
			std::vector<int>	prevSolution;
			/** Aktualnie wyliczany koszt */
			float				currCost;
			/** Aktualnie przetwarzane rozwiazanie */
			std::vector<int>	currSolution;
			/** Koszt najlepszego znalezionego rozwiazania */
			float				bestCost;
			/** Najlepsze znalezione rozwiazanie */
			std::vector<int>	bestSolution;
			/** Czas obliczen w ms */
			DWORD				computationTime;
		public:
			/** -1 = liczba wierzcholkow */
			SimulatedAnnealing(Graph* g, int maxInnerIter = -1, 
				float startTemp = 10.0f, float stopTemp = 1.0E-4f, float coolingFactor = 0.9f)
			{
				this->g = g;
				INNER_ITER_MAX = (maxInnerIter == -1) ? g->V() : maxInnerIter;
				START_TEMP = startTemp;
				STOP_TEMP = stopTemp;
				COOLING_FACTOR = coolingFactor;
			}

			void compute()
			{
				computationTime = GetTickCount();;
				// inicjacja
				currSolution.resize(g->V());
				for(int i=0; i<g->V(); i++)
					currSolution[i] = i;
				currCost = g->hamiltonLength(currSolution);

				prevSolution = currSolution;
				prevCost = currCost;
				bestSolution = currSolution;
				bestCost = currCost;

				temp = START_TEMP;

				while (temp > STOP_TEMP) 
				{
					innerInter = 0;
					while (innerInter < INNER_ITER_MAX) 
					{
						innerInter++;

						// generujemy nowe rozwiazanie
						currSolution = prevSolution;
						std::swap(currSolution[rand()%g->V()], currSolution[rand()%g->V()]);
						currCost = g->hamiltonLength(currSolution);
						float costDiff = currCost - prevCost;

						/**
						* Wywolywana w sytuacji, gdy biezace rozwiazanie ma lepszy
						* koszt od poprzedniego
						*/
						if (costDiff <= 0 ||
						/**
						* Wywolywane w sytuacji, gdy biezace rozwiazanie ma gorszy
						* koszt od poprzedniego, jednak spelnione jest
						* prawdopodobienstwo zamiany rozwiania pomimo gorszego kosztu
						*/
						randf() < expf(-costDiff/temp)) 
						{
							prevSolution = currSolution;
							prevCost = currCost;
							// Aktualizujemy najlepsze rozwiazanie
							if (currCost < bestCost) 
							{
								bestCost = currCost;
								bestSolution = currSolution;
							}
						}
					} // while (innerInter < INNER_ITER_MAX) 
					temp *= COOLING_FACTOR; // nowa wartosc temperatury
				} // while(temp < STOP_TEMP)
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
				return bestSolution;
			}

			/** Zwraca nazwe algorytmu */
			inline const char* getAlgorithmName() const
			{
				return "SimulatedAnnealing";
			}

			/** Zwraca czas obliczen w ms */
			inline DWORD getComputationTime() const
			{
				return computationTime;
			}
		};
	};
}; // namespace WMH
