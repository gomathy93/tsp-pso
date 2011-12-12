#pragma once

#include "stdafx.h"
#include "Graph.h"

//////////////////////////////////////////////////////////////////
// Na razie nic z tego nie jest uzywane							//
// to jest wg:													//
// http://clerc.maurice.free.fr/pso/pso_tsp/ i tam pdf w zipie	//
//////////////////////////////////////////////////////////////////
namespace WMH {
	namespace PSO {

		/** Zamiana 2 punktow na sciezce */
		class PointSwap {
		public:
			// zamien i
			int	i;
			// na j
			int j;

			/** Nie sprawdza czy indeksy sa poprawne tj. nierowne i w zakresie 0..N-1 */
			PointSwap(int i, int j) {
				this->i = i;
				this->j = j;
			}

			inline bool equal(const PointSwap& swap2) const {
				return (swap2.i == i && swap2.j == j);
			}

			inline bool neg_equal(const PointSwap& swap2) const {
				return (swap2.i == j && swap2.j == i);
			}
		};

		/** Klasa reprezentujaca zamiany elementow (predkosc) */
		class Velocity {
			std::vector<PointSwap>	elems;
		public:
			Velocity(const std::vector<PointSwap>& swaps) {
				elems = swaps;
			}

			Velocity(const Velocity& v2) {
				elems = v2.elems;
			}

			inline PointSwap operator[](int index) const {
				return elems[index];
			}

			inline size_t size() const {
				return elems.size();
			}

			/** Negacja */
			inline Velocity neg() {
				Velocity negated(*this);
				for(size_t i=0; i<elems.size(); i++)
					std::swap(negated.elems[i].i, negated.elems[i].j);
				return negated;
			}

			/** Tworzy sume logiczna dwoch predkosci (v + ~v == pusty)*/
			Velocity add(const Velocity& v) {
				return Velocity(v); // TODO: stub
			}
		};

		/** Klasa reprezentujaca cykl Hamiltona (polozenie),
			indeksowanie 0...N-1 dla N wierzcholkow */
		class Position {

			// zablokowany
			Position() {}

			std::vector<int>	indices;
			const Graph*		g;
		public:			
			Position(const Graph* g) {
				this->g = g;
				indices.resize(g->V());
				for(size_t i=0; i<indices.size(); i++)
					indices[i] = i;
			}

			inline int operator[](int index) const {
				return indices[index];
			}

			/** Pozycja + predkosc */
			inline Position add(const Velocity& v) {
				for(size_t i=0; i<v.size(); i++) {
					PointSwap& swap = v[i];
					std::swap(indices[swap.i], indices[swap.j]);
				}
			}
		
			float cost() const {
				float sum = 0.0f;
				for(size_t i=0; i<indices.size()-1; i++) // TODO: dodac duzy koszt jak nie ma polaczenia
					sum += g->getDist(indices[i], indices[i+1]);
				sum += g->getDist(indices[indices.size()-1], indices[0]);
				return sum;
			}
		};
	}; // namespace WMH
}; // namespace PSO
