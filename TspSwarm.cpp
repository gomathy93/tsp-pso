#include "stdafx.h"

#include "TspSwarm.h"

WMH::PSO::TspSwarm::TspSwarm(Graph* g, unsigned int particlesCount, 
							 int maxNoChange, int maxIter) {
	noChange = 0;
	iter = 0;
	NOCHANGE_MAX = maxNoChange;
	ITER_MAX = maxIter;
	this->g = g;
	bestFit = MAX_FLOAT;
	particles.resize(particlesCount);
}


void WMH::PSO::TspSwarm::compute() {
	for(unsigned int i=0; i<particles.size(); i++) 
		particles[i].init(this);

	while(noChange < NOCHANGE_MAX && iter < ITER_MAX) {
		noChange++;
		iter++;
		for(unsigned int i=0; i<particles.size(); i++)	particles[i].update();
	}
}

std::ostream& WMH::PSO::operator << (std::ostream& stream, const WMH::PSO::TspSwarm& swarm)
{
	stream << "Best result: " << swarm.bestFit << std::endl;
	stream << "Best path:" << std::endl;
	std::vector<int> path = swarm.getBestSolution();
	for(unsigned int i=0; i<path.size(); i++)
		stream << path[i] << " ";
	stream << std::endl;
	stream << "Iterations: " << swarm.iter << std::endl;
	return stream;
}
