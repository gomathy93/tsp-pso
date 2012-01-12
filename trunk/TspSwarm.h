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
			/** Ogólna liczba iteracji */
			int			iter;
		public:
			// Wartosci domyslne
			static const float DEF_C1; // C++ jest glupie, to trzeba zadeklarowac w .cpp
			static const float DEF_C2;
			static const float DEF_OMEGA;
			static const int DEF_REHOPE; // aby tego nie robic ustawic ten parametr na wysoka wartosc
			static const int DEF_MAXNOCHANGE;

			/** Tworzy roj rozwiazujacy TSP dla grafu g */
			TspSwarm(const Graph* g, int particlesCount, 
				float C1 = DEF_C1, float C2 = DEF_C2, float omega = DEF_OMEGA, 
				int particleRehope = DEF_REHOPE, int maxNoChange = DEF_MAXNOCHANGE);
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
