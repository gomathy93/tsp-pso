#pragma once

#include "stdafx.h"

#include "Particle.h"
#include "Position.h"
#include "Graph.h"
#include "Algo.h"

namespace WMH {
	namespace PSO {
		/** Klasa reprezentujaca roj */
		class TspSwarm : public iAlgo {
			friend class Particle;
			/** Maksymalna liczba iteracji ktora nie poprawia globalnego wyniku */
			int NOCHANGE_MAX;
			/** Zablokowany - jakis problem trzeba rozwiazywac */
			TspSwarm() {}
			/** Czas obliczen w ms */
			DWORD computationTime;

			void assign(const TspSwarm& t2) {
				NOCHANGE_MAX = t2.NOCHANGE_MAX;
				computationTime = t2.computationTime;
				g = t2.g;
				particles = t2.particles;
				best = t2.best;
				bestFit = t2.bestFit;
				noChange = t2.noChange;
			}
		protected:
			/** Graf dla ktorego przeprowadzane sa obliczenia */
			const Graph*	g;
			/** List czastek */
			std::vector<Particle> particles;
			/** Najlepsze znalezione rozwiazanie (globalne) */
			Position	best;
			/** Miara dopasowania najlepszego rozwiazania */
			float		bestFit;
			/** Liczba iteracji ktora nie poprawila wyniku */
			int			noChange;
		public:
			/** Tworzy roj rozwiazujacy TSP dla grafu g */
			TspSwarm(const Graph* g, unsigned int particlesCount, int maxNoChange = 1000);
			/** Konstruktor kopiujacy */
			TspSwarm(const TspSwarm& t2) {
				assign(t2);
			}
			/** Operator przypisania */
			inline TspSwarm& operator=(const TspSwarm& t2) {
				if (this == &t2) return *this;
				assign(t2);
				return *this;
			}
			
			/** Rozpoczyna obliczenia */
			void compute();
			
			/** Zwraca koszt najlepszego rozwiazania */
			inline float getBestCost() const {
				return bestFit;
			}
			
			/** Zwraca najlepsze rozwiazanie */
			inline std::vector<int> getBestSolution() const {
				return best.getIndices();
			}
			
			/** Zwraca nazwe algorytmu */
			inline const char* getAlgorithmName() const {
				return "PSO\t\t";
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
	}; // namespace PSO
}; // namespace TSP
