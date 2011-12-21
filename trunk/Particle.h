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
		public:
			/** Roj do ktorego przynalezy czastka */
			TspSwarm*		swarm;
			/** Pozycja czastki */
			Position	position;
			/** Predkosc czastki */
			Velocity	speed;
			/** Tworzy czastke nieprzypisana do zadnego roju */
			Particle() {
				swarm = NULL;
				bestFit = MAX_FLOAT;
			}
			/** Tworzy czastke przynalezaca do roju swarm */
			void init(TspSwarm* swarm);
			/** Aktualizuje predkosc i polozenie w kolejnej iteracji oraz najlepsze wyniki */
			void update();
		};
	}; // namespace PSO
}; // namespace WMH
