#pragma once

#include "stdafx.h"
#include "utils.h"

namespace WMH {
	namespace PSO {
		/** Przechowuje liste wartosci (cech) o konkretnym wymiarze */
		class Features {
			/** Lista wartosci */
			std::vector<float>	values;

			inline void assign(const Features& f2)
			{
				unsigned int n = f2.values.size();
				values.resize(n);
				for(unsigned int i=0; i<n; i++)
					values[i] = f2.values[i];
			}
		public:
			/** Tworzy pusta liste cech */
			Features() {}
			/** Tworzy liste cech o wymiarze dimension */
			Features(unsigned int dimension) {
				values.resize(dimension);
				for(unsigned int i=0; i<dimension; i++)
					values[i] = 0.0f;
			}

			/** Konstruktor kopiujacy - unikamy glupich bledow */
			Features(const Features& f2) {
				assign(f2);
			}

			/** Operator przypisania - unikamy glupich bledow */
			Features& operator=(const Features& f2) { 
				assign(f2);
				return *this;
			}

			inline float& operator[](int index) {
				return values[index];
			}

			inline float operator[](int index) const {
				return values[index];
			}

			inline unsigned int count() const {
				return values.size();
			}

			/** Przypisuje kazdemu elementowi losowe wartosci z zakresu [0, wymiar] */
			inline void randomize(int dim) {
				values.resize(dim);
				for(int i=0; i<dim; i++)
					values[i] = randf(0.0f, static_cast<float>(dim));
			}

			/** Przeksztalca wartosci na permutacje (na potrzeby TSP) */
			inline std::vector<int> toPermut() const {
				unsigned int i, n = values.size();
				std::vector<int> res(n);
				for(i=0; i < n; i++)
					res[i] = i;

				for(i=0; i < n; i++)
					std::swap(res[i], res[static_cast<int>(values[i])%n]);

				return res;
			}
		};
	}; // namespace PSO
}; // namespace WMH
