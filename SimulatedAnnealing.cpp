#include "stdafx.h"

#include "SimulatedAnnealing.h"

WMH::SA::SimulatedAnnealing::SimulatedAnnealing(const Graph* g, int maxInnerIter, 
				   float startTemp, float stopTemp,
				   float coolingFactor) {
	this->g = g;
	INNER_ITER_MAX = (maxInnerIter == -1) ? g->V() : maxInnerIter;
	START_TEMP = startTemp;
	STOP_TEMP = stopTemp;
	COOLING_FACTOR = coolingFactor;
}

void WMH::SA::SimulatedAnnealing::compute(bool saveResults) {
	computationTime = GetTickCount();;
	// inicjacja
	currSolution.resize(g->V());
	for(int i=0; i<g->V(); i++) currSolution[i] = i;
	currCost = g->hamiltonLength(currSolution);

	prevSolution = currSolution;
	prevCost = currCost;
	bestSolution = currSolution;
	bestCost = currCost;

	iterResults.clear();
	iter = 0;
	if(saveResults)
		iterResults.push_back(IterCost(iter, bestCost));
	
	temp = START_TEMP;
	while (temp > STOP_TEMP) {
		iter++;
		innerInter = 0;
		while (innerInter < INNER_ITER_MAX) {
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
			randf() < expf(-costDiff/temp)) {
				prevSolution = currSolution;
				prevCost = currCost;
				// Aktualizujemy najlepsze rozwiazanie
				if (currCost < bestCost) {
					bestCost = currCost;
					bestSolution = currSolution;

					if(saveResults) {
						size_t count = iterResults.size();
						if(count > 0 && iterResults[count-1].iter == iter)
							iterResults[count-1].cost = bestCost; // aktualizacja
						else
							iterResults.push_back(IterCost(iter, bestCost));
					}
				}
			}
		} // while (innerInter < INNER_ITER_MAX) 
		temp *= COOLING_FACTOR; // nowa wartosc temperatury		
	} // while(temp < STOP_TEMP)
	
	if(saveResults)
		iterResults.push_back(IterCost(iter, bestCost));
	computationTime = GetTickCount() - computationTime;
}
