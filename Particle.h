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
			friend class TspSwarm;
			/** Identyfikator czastki (dla debugu) */
			int			id;
			/** Najlepsze znalezione rozwiazanie (przez ta czastke) */
			Position	best;
			/** Miara dopasowania najlepszego rozwiazania */
			float		bestFit;
			/** Przez ile ostatnich iteracji ta czastka nie poprawila swojego wyniku */
			int			noChange;

			void assign(const Particle& p2) {
				best = p2.best;
				bestFit = p2.bestFit;
				swarm = p2.swarm;
				position = p2.position;
				speed = p2.speed;
				id = p2.id;
				noChange = p2.noChange;
			}
		protected:
			// wlasciwie to wspolczynniki powinny byc w swarm ALE
			// umieszczajac je tutaj unikamy kazdorazowego szukania
			// adresu wskaznika (swarm) = szybciej
			/** Wspolczynnik bezwladnosci */ 
			static float OMEGA;
			/** Stale przyspieszenia */
			static float C1, C2;
			/** Jesli po tylu iteracjach czasteczka nie znajdzie lepszej pozycji to zostanie rozrzucona ponownie */
			static int REHOPE;
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
				noChange = 0;
				id = -1;
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

			/** Przypisuje czastke do roju swarm, losuje polozenie */
			void init(TspSwarm* swarm, int id);
			/** Aktualizuje predkosc i polozenie w kolejnej iteracji oraz najlepsze wyniki */
			void update();
		};
	}; // namespace PSO
}; // namespace WMH
