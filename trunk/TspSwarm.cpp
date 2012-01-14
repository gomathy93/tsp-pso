#include "stdafx.h"

#include "TspSwarm.h"

const float WMH::PSO::TspSwarm::DEF_C1 = 0.45895f;
const float WMH::PSO::TspSwarm::DEF_C2 = -0.078222f;
const float WMH::PSO::TspSwarm::DEF_OMEGA = 1.43f;
const int WMH::PSO::TspSwarm::DEF_REHOPE = 80; // aby tego nie robic ustawic ten parametr na wysoka wartosc
const int WMH::PSO::TspSwarm::DEF_MAXNOCHANGE = 1000;

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


void WMH::PSO::TspSwarm::compute(bool saveResults) {
	computationTime = GetTickCount();
	noChange = 0;
	iter = 0;
	iterResults.clear();

	for(unsigned int i=0; i<particles.size(); i++) 
		particles[i].init(this, i + 1);

	float prevBestFit = bestFit;
	while(noChange < NOCHANGE_MAX) {
		noChange++;
		for(unsigned int i=0; i<particles.size(); i++) particles[i].update();
		
		if(saveResults && bestFit != prevBestFit) {
			iterResults.push_back(IterCost(iter, bestFit));
			prevBestFit = bestFit;
		}
		iter++;
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

	if(saveResults)
		iterResults.push_back(IterCost(iter, bestFit));
	computationTime = GetTickCount() - computationTime;
}
