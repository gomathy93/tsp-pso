#include "stdafx.h"

#include "TspSwarm.h"

WMH::PSO::TspSwarm::TspSwarm(const Graph* g, unsigned int particlesCount, int maxNoChange) {
	NOCHANGE_MAX = 300;
	this->g = g;
	bestFit = MAX_FLOAT;
	particles.resize(particlesCount);
	computationTime = 0;
	noChange = 0;
}


void WMH::PSO::TspSwarm::compute() {
	computationTime = GetTickCount();
	noChange = 0;
	for(unsigned int i=0; i<particles.size(); i++) 
		particles[i].init(this);

	while(noChange < NOCHANGE_MAX) {
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++)	particles[i].update();
	}

	noChange = 0;
	while(noChange < NOCHANGE_MAX/2){
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++){
			particles[i].position=particles[i].best;
			particles[i].speed*0.7;	
		}
		for(unsigned int i=0; i<particles.size(); i++)
			particles[i].update();
	}
	noChange=0;
		while(noChange < NOCHANGE_MAX/2){
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++){
			particles[i].position=particles[i].best;
			particles[i].speed;	
		}
		for(unsigned int i=0; i<particles.size(); i++)
			particles[i].update();
	}
			noChange=0;
		while(noChange < NOCHANGE_MAX/2){
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++){
			particles[i].position=particles[i].best;
			particles[i].speed;	
		}
		for(unsigned int i=0; i<particles.size(); i++)
			particles[i].update();
	}

	computationTime = GetTickCount() - computationTime;
}
