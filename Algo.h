#pragma once

#include "stdafx.h"

namespace WMH {
	/** Interfejs po ktorym maja dziedziczyc wszystkie 
	  * algorytmy rozwiazywania problemu TSP 
	  */
	class iAlgo
	{
	public:
		virtual void compute() = 0;
		virtual const char* getAlgorithmName()const = 0;
		virtual float getBestCost() const = 0;
		virtual std::vector<int> getBestSolution() const = 0;
	};
}; // namespace WMH
