#include "stdafx.h"

namespace PSO
{
	// forward definition
	class Swarm;

	class Particle
	{
		/** Roj do ktorego przynalezy czastka */
		Swarm*	swarm;
	public:
		/** Tworzy czastke przynalezaca do roju swarm */
		Particle(Swarm* swarm)
		{
			this->swarm = swarm;
		}
		/** Pozycja czastki */
		vector<double>	position;
		/** Predkosc czastki */
		vector<double>	velocity;
	};
}; // namespace PSO
