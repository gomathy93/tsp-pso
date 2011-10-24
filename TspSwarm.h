#pragma once

#include "stdafx.h"

#include "Particle.h"
#include "Features.h"
#include "Graph.h"

namespace PSO {
	/** Klasa reprezentujaca roj */
	class TspSwarm	{
		/** Maksymalna liczba iteracji ktora nie poprawia globalnego wyniku */
		static const int NOCHANGE_MAX = 1000;
		/** Zablokowany - jakis problem trzeba rozwiazywac */
		TspSwarm() {}
	public:
		/** Graf dla ktorego przeprowadzane sa obliczenia */
		Graph*	g;
		/** List czastek */
		std::vector<Particle> particles;
		/** Najlepsze znalezione rozwiazanie (globalne) */
		Features	best;
		/** Miara dopasowania najlepszego rozwiazania */
		float		bestFit;
		/** Liczba iteracji ktora nie poprawila wyniku */
		int			noChange;

		/** Tworzy roj rozwiazujacy TSP dla grafu g */
		TspSwarm(Graph* g);
		/** Rozpoczyna obliczenia */
		void compute();
	};
}; // namespace PSO
