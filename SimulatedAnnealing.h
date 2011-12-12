#pragma once

#include "stdafx.h"
#include "Algo.h"
#include "utils.h"

namespace WMH {
	namespace SA {
		/** TSP metoda symulowanego wyzarzania */
		class SimulatedAnnealing : public iAlgo	{
			/** Przetwarzany graf */
			const Graph*		g;
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
			SimulatedAnnealing(const Graph* g, int maxInnerIter = -1, 
				float startTemp = 100.0f, float stopTemp = 1.0E-4f, 
				float coolingFactor = 0.9f);

			void compute();

			/** Zwraca koszt najlepszego rozwiazania */
			inline float getBestCost() const {
				return bestCost;
			}

			/** Zwraca najlepsze rozwiazanie */
			inline std::vector<int> getBestSolution() const	{
				return bestSolution;
			}

			/** Zwraca nazwe algorytmu */
			inline const char* getAlgorithmName() const	{
				return "SimulatedAnnealing";
			}

			/** Zwraca czas obliczen w ms */
			inline DWORD getComputationTime() const	{
				return computationTime;
			}

			/** Zwraca graf na ktorym wykonywane sa obliczenia */
			virtual const Graph* getGraph() const {
				return g;
			}
		};
	};
}; // namespace WMH
