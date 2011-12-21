#pragma once

#include "stdafx.h"

#include "Velocity.h"

namespace WMH {
	namespace PSO {
		/** Negacja */
		Velocity Velocity::operator~() const {
			std::vector<PointSwap> swaps;
			for(int i=size()-1; i>=0; i--)
				swaps.push_back(elems[i]);
			return Velocity(swaps);
		}

		/** Tworzy sume logiczna dwoch predkosci (v + ~v == pusty)*/
		Velocity Velocity::operator+(const Velocity& v) const {
			Velocity added(*this);
			added += v;
			return added;
		}

		Velocity& Velocity::operator+=(const Velocity& v) {
			// stara wersja 
			/*for(size_t i=0; i<v.size(); i++) {
				if(size() > 0 && elems.back() == v[i])
					elems.pop_back();
				else
					elems.push_back(v[i]);
			}
			return *this;*/
			for(size_t i=0; i<v.size(); i++) {
				std::vector<PointSwap>::iterator it = elems.begin(), end = elems.end();
				bool found = false;
				while(it != end) {
					if(*it == v[i]) {
						elems.erase(it);
						found = true;
						break;
					}
					++it;
				}
				if(!found) elems.push_back(v[i]);
			}
			return *this;
		}

		/** Mnozenie przez liczbe (dowolna zmiennoprzecinkowa) */
		Velocity Velocity::operator*(const float C) const {
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
				Velocity vel;
				int k = static_cast<int>(C);
				for(int i=0; i<k; i++)
					vel += *this;
				return vel * (C - k);
			}
			if(C < 0.0f) // to samo co w przypadku dodatniego C tylko z negacja
				return ~(*this) * -C;
			assert(0);
		}
	}; // namespace WMH
}; // namespace PSO
