#include "stdafx.h"

#include "TspSwarm.h"

PSO::TspSwarm::TspSwarm(Graph* g) {
	noChange = 0;
	this->g = g;
	bestFit = MAX_FLOAT;
}


void PSO::TspSwarm::compute() {
	particles.resize(g->V());
	for(int i=0; i<g->V(); i++)	particles[i].init(this);

	while(noChange < NOCHANGE_MAX) {
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++)	particles[i].update();
		printf("Shortest path: %f\n", bestFit);
	}
}
