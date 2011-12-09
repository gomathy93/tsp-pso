#pragma once

#include "Algo.h"
#include "Graph.h"

namespace WMH {
	namespace RS {
		/** Losowe przeszukiwanie przestrzeni rozwiazan (TSP) */
		class RandomSearch : public iAlgo
		{
			/** Maksymalna liczba iteracji */
			int		ITER_MAX;
			/** Graf dla ktorego przeprowadzane sa obliczenia */
			Graph*	g;
		public:
			/** Inicjuje przeszukiwanie rozwiazan dla TSP dla grafu g */
			RandomSeach(Graph* g, int maxIter = 1000000)
			{
				ITER_MAX = maxIter;
				this->g = g;
			}


			void compute()
			{

			}
		};
	}; // namespace RS
}; // namespace WMH
