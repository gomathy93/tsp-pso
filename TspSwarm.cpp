#include "stdafx.h"

#include "TspSwarm.h"

WMH::PSO::TspSwarm::TspSwarm(	const Graph* g, int particlesCount, 
								float C1, float C2, float omega, 
								int particleRehope, int maxNoChange) {
	this->g = g;
	bestFit = MAX_FLOAT;
	particles.resize(particlesCount);
	computationTime = 0;
	noChange = 0;
	iter = 0;
	Particle::C1 = C1;
	Particle::C2 = C2;
	Particle::OMEGA = omega;
	Particle::REHOPE = particleRehope;
	NOCHANGE_MAX = maxNoChange;
}


void WMH::PSO::TspSwarm::compute() {
	computationTime = GetTickCount();
	noChange = 0;
	iter = 0;
	for(unsigned int i=0; i<particles.size(); i++) 
		particles[i].init(this, i + 1);

	while(noChange < NOCHANGE_MAX) {
		iter++;
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++) particles[i].update();
	}

	// TODO: wykomentowalem Romana
	/*noChange = 0;
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
	}*/

	computationTime = GetTickCount() - computationTime;
}
