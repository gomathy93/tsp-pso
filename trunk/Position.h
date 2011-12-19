#pragma once

#include "stdafx.h"
#include "Graph.h"

namespace WMH {
	namespace PSO {
		/** Klasa reprezentujaca cykl Hamiltona (polozenie),
			indeksowanie 0...N-1 dla N wierzcholkow */
		class Position {

			// zablokowany
			Position() {}

			std::vector<int>	indices;
			const Graph*		g;

			inline size_t getVerticleIndex(int verticle) const {
				for(size_t i=0; i<indices.size(); i++)
					if(indices[i] == verticle)
						return i;
				assert(0);
			}
		public:			
			Position(const Graph* g) {
				this->g = g;
				indices.resize(g->V());
				
				for (size_t i = 0; i<size(); i++) { // losowa permutacja
					int j = rand() % (i + 1);
					indices[i] = indices[j];
					indices[j] = i;
				}
			}

			inline int operator[](int index) const {
				return indices[index];
			}

			inline size_t size() const {
				return indices.size();
			}

			/** Pozycja + predkosc */
			Position operator+(const Velocity& v) const {
				Position pos(*this);
				for(size_t i=0; i<v.size(); i++) {
					const PointSwap& swap = v[i];
					size_t i1 = pos.getVerticleIndex(swap.i);
					size_t i2 = pos.getVerticleIndex(swap.j);
					std::swap(pos.indices[i1], pos.indices[i2]);
				}
				return pos;
			}

			/** Pozycja - pozycja */
			Velocity operator-(const Position& p2) const { // TODO: czy to dobrze dziala?
				if(size() != p2.size()) return Velocity();
				std::list<PointSwap> swaps;
				std::vector<int> indCopy(indices);

				for(size_t i=0; i<size(); i++) {
					if(indCopy[i] != p2[i]) {
						size_t i2 = getVerticleIndex(p2[i]);
						indCopy[i2] = indCopy[i];
						indCopy[i] = p2[i];
						swaps.push_front(PointSwap(indices[i], p2[i]));
					}
				}
				return Velocity(std::vector<PointSwap>(swaps.begin(), swaps.end()));
			}
		
			/** Zwraca koszt sciezki wg wag w grafie */
			float cost() const {
				float sum = 0.0f;
				for(size_t i=0; i<indices.size()-1; i++) // TODO: dodac duzy koszt jesli nie ma polaczenia
					sum += g->getDist(indices[i], indices[i+1]);
				sum += g->getDist(indices[indices.size()-1], indices[0]);
				return sum;
			}
		};
	}; // namespace WMH
}; // namespace PSO
