#include "stdafx.h"

#include "Particle.h"
#include "TspSwarm.h"
#include "utils.h"

float WMH::PSO::Particle::OMEGA = WMH::PSO::TspSwarm::DEF_OMEGA;
float WMH::PSO::Particle::C1 = WMH::PSO::TspSwarm::DEF_C1;
float WMH::PSO::Particle::C2 = WMH::PSO::TspSwarm::DEF_C2;
int WMH::PSO::Particle::REHOPE = WMH::PSO::TspSwarm::DEF_REHOPE;

void WMH::PSO::Particle::init(TspSwarm* swarm, int id) {
	this->swarm = swarm;
	this->id = id;
	position.randomize(swarm->g->V());
}

void WMH::PSO::Particle::update() {
	Position& bestGlobal = swarm->best;
	const Graph* graph = swarm->g;

	noChange++; // ilosc iteracji braku poprawy TEJ czastki
	float fit = position.cost(graph);
	if(fit < swarm->bestFit || swarm->best.size() == 0) {
		swarm->best = position;
		swarm->bestFit = fit;
		swarm->noChange = 0;
	}
	if(fit < bestFit || best.size() == 0) {
		best = position;
		bestFit = fit;
		noChange = 0;
	}

	if(noChange < REHOPE) {
		// Zmiana polozenia i predkosci
		speed = speed * randf(min(OMEGA, 0.0f), max(OMEGA, 0.0f)) + 
			(best - position) * randf(min(C1, 0.0f), max(C1, 0.0f)) + 
			(bestGlobal - position) * randf(min(C2, 0.0f), max(C2, 0.0f));
		position += speed;
	} else {
		speed.zero();
		position.randomize(graph->V());
		noChange = 0;
	}
}
