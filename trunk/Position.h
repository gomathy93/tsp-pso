#pragma once

#include "stdafx.h"

namespace WMH {
	
	class Graph; // forward

	namespace PSO {

		class Velocity; // forward

		/** Klasa reprezentujaca cykl Hamiltona (polozenie),
			indeksowanie 0...N-1 dla N wierzcholkow */
		class Position {

			int*	indices;
			size_t	N;

			inline size_t getVerticleIndex(int verticle) const {
				for(size_t i=0; i<N; i++)
					if(indices[i] == verticle)
						return i;
				assert(0);
			}
		public:			
			Position() {
				indices = NULL;
				N = 0;
			}

			~Position() {
				if(indices) delete [] indices;
			}

			void randomize(size_t size);

			inline int operator[](int index) const {
				return indices[index];
			}

			inline size_t size() const {
				return N;
			}

			inline std::vector<int> getIndices() const {
				return std::vector<int>(indices, indices + N);
			}

			/** Pozycja + predkosc */
			Position& operator+=(const Velocity& v);
			/** Pozycja - pozycja */
			Velocity operator-(const Position& p2) const;
			/** Zwraca koszt sciezki wg wag w grafie */
			float cost(const Graph* g) const;
		};
	}; // namespace WMH
}; // namespace PSO
