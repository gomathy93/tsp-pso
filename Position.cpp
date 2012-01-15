#pragma once

#include "stdafx.h"

#include "Position.h"
#include "Velocity.h"
#include "Graph.h"

namespace WMH {
	namespace PSO {

		/** Pobiera indeks gdzie znajduje sie wierzcholek o danym numerze na liscie */
		size_t Position::getVerticleIndex(int verticle) const {
			// TODO: to mozna zoptymalizowac do O(1)
			for(size_t i=0; i<N; i++)
				if(indices[i] == verticle)
					return i;
			assert(0);
		}

		/** Przypisuje zawartosc innej pozycji, 
		  * Uzywana do konstruktora kopiujacego, operatora przypisania 
		  */
		void Position::assign(const Position& p) {
			if(indices) delete [] indices;
			if(p.indices) {
				indices = new int[p.N];
				memcpy(indices, p.indices, sizeof(int) * p.N);
				N = p.N;
			} else {
				indices = NULL;
				N = 0;
			}
		}
		
		/** Losuje zawartosc wektora polozenia */
		void Position::randomize(size_t N) {
				if(indices) delete [] indices;
				indices = new int[N];
				this->N = N;
				for (size_t i = 0; i<N; i++) { // losowa permutacja
					int j = rand() % (i + 1);
					indices[i] = indices[j];
					indices[j] = i;
				}
			}

		/** Pozycja + predkosc */
		Position& Position::operator+=(const Velocity& v) {
			for(size_t i=0; i<v.size(); i++) {
				const PointSwap& swap = v[i];
				size_t i1 = getVerticleIndex(swap.i);
				size_t i2 = getVerticleIndex(swap.j);
				std::swap(indices[i1], indices[i2]);
			}
			return *this;
		}

		/** Pozycja - pozycja */
		Velocity Position::operator-(const Position& p2) const {
			if(size() != p2.size()) return Velocity();
			std::list<PointSwap> swaps;
			int* indCopy = new int[N];
			
			memcpy(indCopy, indices, N * sizeof(int));
			for(size_t i=0; i<N-1; i++) {
				if(indices[i] != p2[i]) {
					size_t i2 = getVerticleIndex(p2[i]);
					
					int temp1 = indices[i2];

					indices[i2] = indices[i];
					indices[i]=temp1;
					
					swaps.push_front(PointSwap(indices[i2], indices[i]));
				}
			}
			memcpy(indices, indCopy, N * sizeof(int));
			delete [] indCopy;
			return Velocity(std::vector<PointSwap>(swaps.begin(), swaps.end()));
		
		}

			
		/** Zwraca koszt sciezki wg wag w grafie */
		float Position::cost(const Graph* g) const {
			if(N == 0) return 0.0f;
			float sum = 0.0f;
			for(size_t i=0; i<N-1; i++)
					sum += g->getDist(indices[i], indices[i+1]);
			sum += g->getDist(indices[N-1], indices[0]);
			return sum;
		}

		/** Zapisuje pozycje do strumienia */
		std::ostream& operator << (std::ostream& stream, const Position& p) {
			for(size_t i=0; i<p.N; i++)
				stream << p[i] << ' ';
			stream << std::endl;
			return stream;
		}
	}; // namespace WMH
}; // namespace PSO
