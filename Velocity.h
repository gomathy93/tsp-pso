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

			PointSwap(const PointSwap& ps) {
				i = ps.i;
				j = ps.j;
			}

			inline PointSwap& operator=(const PointSwap& ps) {
				i = ps.i;
				j = ps.j;
				return *this;
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

			inline Velocity& operator=(const Velocity& v2) {
				if (this == &v2) return *this;
				elems = v2.elems;
				return *this;
			}

			inline PointSwap operator[](int index) const {
				return elems[index];
			}

			inline size_t size() const {
				return elems.size();
			}

			/** Negacja */
			Velocity operator~() const;
			/** Tworzy sume logiczna dwoch predkosci (v + ~v == pusty)*/
			Velocity operator+(const Velocity& v) const;
			Velocity& operator+=(const Velocity& v);
			/** Mnozenie przez liczbe (dowolna zmiennoprzecinkowa) */
			Velocity operator*(const float C) const;

			/** Zapisuje liste podmian do strumienia */
			friend std::ostream& operator << (std::ostream& stream, const Velocity& v);
		};
	}; // namespace WMH
}; // namespace PSO
