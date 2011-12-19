#pragma once

#include "stdafx.h"

//////////////////////////////////////////////////////////////////
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

			inline bool operator == (const PointSwap& swap2) const {
				return (swap2.i == i && swap2.j == j) || (swap2.i == j && swap2.j == i);
			}
		};

		/** Klasa reprezentujaca zamiany elementow (predkosc) */
		class Velocity {
			std::vector<PointSwap>	elems;
		public:
			Velocity() {}

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
			Velocity operator~() const {
				std::vector<PointSwap> swaps;
				for(int i=size()-1; i>=0; i--)
					swaps.push_back(elems[i]);
				return Velocity(swaps);
			}

			/** Tworzy sume logiczna dwoch predkosci (v + ~v == pusty)*/
			Velocity operator+(const Velocity& v) const { // TODO: czy to na pewno ma tak dzialac?
				Velocity added(*this);
				for(size_t i=0; i<v.size(); i++) {
					if(added.size() > 0 && added.elems.back() == v[i])
						added.elems.pop_back();
					else
						added.elems.push_back(v[i]);
				}
				return added;
			}

			/** Mnozenie przez liczbe (dowolna zmiennoprzecinkowa) */
			Velocity operator*(const float C) const {
				if(C == 0.0f) // zwracamy pusty
					return Velocity(); 
				if(C > 0.0f && C < 1.0f) { // obcinamy do |cv|
					if(size() == 0) return Velocity();
					std::vector<PointSwap> swaps;
					size_t treshold = static_cast<unsigned int>(floor(C * size())) + 1;
					for(size_t i=0; i<treshold; i++)
						swaps.push_back(elems[i]);
					return Velocity(swaps);
				}
				if(C >= 1.0f) { // jesli C=5.2 to 5 razy dodajemy, raz obcinamy do |0.2*v|
					Velocity vel(*this);
					int k = static_cast<int>(C);
					for(int i=0; i<k; i++)
						vel += vel;
					return vel * (C - k);
				}
				if(C < 0.0f) // to samo co w przypadku dodatniego C tylko z negacja
					return ~(*this) * -C;
				assert(0);
			}
		};
	}; // namespace WMH
}; // namespace PSO
