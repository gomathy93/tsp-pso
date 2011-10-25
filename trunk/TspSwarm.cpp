#include "stdafx.h"

#include "TspSwarm.h"

PSO::TspSwarm::TspSwarm(Graph* g, unsigned int particlesCount) {
	noChange = 0;
	this->g = g;
	bestFit = MAX_FLOAT;
	particles.resize(particlesCount);
}


void PSO::TspSwarm::compute() {
	for(unsigned int i=0; i<particles.size(); i++) particles[i].init(this);

	while(noChange < NOCHANGE_MAX) {
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++)	particles[i].update();
		printf("Shortest path: %f\n", bestFit);
	}
}
