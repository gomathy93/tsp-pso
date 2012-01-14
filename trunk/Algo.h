#pragma once

#include "stdafx.h"

#include "Graph.h"

namespace WMH {

	/** Przechowuje wynik w danej iteracji */
	struct IterCost {
	public:
		IterCost(int iter, float cost) {
			this->iter = iter;
			this->cost = cost;
		}
		/** Iteracja */
		int		iter;
		/** Koszt w tej iteracji */
		float	cost;
	};

	/** Interfejs po ktorym maja dziedziczyc wszystkie 
	  * algorytmy rozwiazywania problemu TSP 
	  */
	class iAlgo {
	public:
		/** Rozpoczyna obliczenia, saveResults - czy ma zapisywac koszt co iles iteracji */
		virtual void compute(bool saveResults) = 0;
		/** Zwraca tablice kosztow w poszczegolnych iteracjach */
		virtual std::vector<IterCost> getCostTable() const = 0;
		/** Zwraca nazwe algorytmu */
		virtual const char* getAlgorithmName() const = 0;
		/** Zwraca najlepszy uzyskany koszt cyklu */
		virtual float getBestCost() const = 0;
		/** Zwraca najlepszy uzyskany cykl */
		virtual std::vector<int> getBestSolution() const = 0;
		/** Zwraca czas dzialania algorytmu w ms */
		virtual DWORD getComputationTime() const = 0;
		/** Zwraca graf na ktorym wykonywane sa obliczenia */
		virtual const Graph* getGraph() const = 0;
	};
}; // namespace WMH
