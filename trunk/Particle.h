#include "stdafx.h"

namespace PSO
{
	// forward definition
	class Swarm;

	class Particle
	{
		/** Roj do ktorego przynalezy czastka */
		Swarm*			swarm;
		/** Najlepsze znalezione rozwiazanie (przez ta czastke) */
		vector<float>	best;
		/** Wspolczynnik bezwladnosci */
		static const float OMEGA;
		/** Stale przyspieszenia */
		static const float C1, C2;
	public:
		/** Tworzy czastke przynalezaca do roju swarm */
		Particle(Swarm* swarm)
		{
			this->swarm = swarm;
		}
		/** Pozycja czastki */
		vector<float>	position;
		/** Predkosc czastki */
		vector<float>	speed;
		/** Aktualizuje predkosc i polozenie w kolejnej iteracji */
		void update()
		{
			vector<float> bestGlobal& = swarm->best;
			for(int i=0; i<speed.size(); i++)
				speed[i] = OMEGA*speed[i] + C1*frand(0.0f, best[i] - position[i]) + C2*frand(0.0f, bestGlobal[i] - position[i]);
			for(int i=0; i<position.size(); i++)
				position[i] += speed[i];
		}
	};
}; // namespace PSO
