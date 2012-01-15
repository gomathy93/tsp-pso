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

			/** Pobiera indeks gdzie znajduje sie wierzcholek o danym numerze na liscie */
			size_t getVerticleIndex(int verticle) const;

			/** Przypisuje zawartosc innej pozycji, 
			  * Uzywana do konstruktora kopiujacego, operatora przypisania 
			  */
			void assign(const Position& p);
		public:		
			
			Position() {
				indices = NULL;
				N = 0;
			}

			Position(size_t N) {
				indices = NULL;
				randomize(N);
			}

			Position(const Position& p) {
				indices = NULL;
				assign(p);
			}

			~Position() {
				if(indices) delete [] indices;
			}

			/** Losuje zawartosc wektora polozenia */
			void randomize(size_t size);

			inline int operator[](int index) const {
				return indices[index];
			}

			inline Position& operator=(const Position& p) {
				if (this == &p) return *this;
				assign(p);
				return *this;
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

			/** Zapisuje pozycje do strumienia */
			friend std::ostream& operator << (std::ostream& stream, const Position& p);
		};
	}; // namespace WMH
}; // namespace PSO
