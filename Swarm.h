#include "stdafx.h"

namespace PSO
{
	/** Klasa reprezentujaca roj */
	class Swarm
	{
		/** Obliczenia skonczone */
		bool	done;
		/** Iteracja */
		int		iter;
		/** Maksymalna liczba iteracji */
		static const int ITER_MAX = 1000000;
		/** Najlepsze znalezione rozwiazanie (globalne) */
		vector<float> best;

		void initParticles(int particlesCount)
		{
		}
	public:
		/** List czastek */
		vector<Particle> particles;

		Swarm(int particlesCount)
		{
			done = false;
			iter = 0;
			initParticles(particlesCount);
		}

		/** Rozpoczyna obliczenia */
		void compute()
		{
			initParticles();

			while(!done)
			{
				iter++;

				for(int i=0; i<particlesCount; i++)
				{
					particles[i].update();
				}

				if(iter > ITER_MAX)
					break;
			}
		}
	};
}; // namespace PSO
