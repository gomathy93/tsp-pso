#pragma once

#include "stdafx.h"

#include "Position.h"
#include "Velocity.h"
#include "utils.h"

namespace WMH {
	namespace PSO {
		// forward-def
		class TspSwarm;

		class Particle {
			/** Najlepsze znalezione rozwiazanie (przez ta czastke) */
			Position	best;
			/** Miara dopasowania najlepszego rozwiazania */
			float		bestFit;
			/** Wspolczynnik bezwladnosci */
			static const float OMEGA;
			/** Stale przyspieszenia */
			static const float C1, C2;

			void assign(const Particle& p2) {
				best = p2.best;
				bestFit = p2.bestFit;
				swarm = p2.swarm;
				position = p2.position;
				speed = p2.speed;
			}
		public:
			/** Roj do ktorego przynalezy czastka */
			TspSwarm*	swarm;
			/** Pozycja czastki */
			Position	position;
			/** Predkosc czastki */
			Velocity	speed;
			/** Tworzy czastke nieprzypisana do zadnego roju */
			Particle() {
				swarm = NULL;
				bestFit = MAX_FLOAT;
			}
			/** Konstruktor kopiujacy */
			Particle(const Particle& p2) {
				assign(p2);
			}
			/** Operator przypisania */
			Particle& operator=(const Particle& p2) {
				if (this == &p2) return *this;
				assign(p2);
				return *this;
			}

			/** Tworzy czastke przynalezaca do roju swarm */
			void init(TspSwarm* swarm);
			/** Aktualizuje predkosc i polozenie w kolejnej iteracji oraz najlepsze wyniki */
			void update();
		};
	}; // namespace PSO
}; // namespace WMH
