#pragma once

#include "stdafx.h"

namespace WMH {
	/** Interfejs po ktorym maja dziedziczyc wszystkie 
	  * algorytmy rozwiazywania problemu TSP 
	  */
	class iAlgo
	{
	public:
		/** Rozpoczyna obliczenia */
		virtual void compute() = 0;
		/** Zwraca nazwe algorytmu */
		virtual const char* getAlgorithmName()const = 0;
		/** Zwraca najlepszy uzyskany koszt cyklu */
		virtual float getBestCost() const = 0;
		/** Zwraca najlepszy uzyskany cykl */
		virtual std::vector<int> getBestSolution() const = 0;
		/** Zwraca czas dzialania algorytmu w ms */
		virtual DWORD getComputationTime() const = 0;
	};
}; // namespace WMH
