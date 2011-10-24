#include "stdafx.h"

#include "Particle.h"
#include "TspSwarm.h"
#include "utils.h"

const float PSO::Particle::OMEGA = 0.72f;
const float PSO::Particle::C1 = 2.0f;
const float PSO::Particle::C2 = 2.0f;

void PSO::Particle::init(TspSwarm* swarm) {
	this->swarm = swarm;
	position.randomize(swarm->g->V());
	speed.randomize(swarm->g->V());
}

void PSO::Particle::update() {
	Features& bestGlobal = swarm->best;
	Graph* graph = swarm->g;

	float fit = graph->hamiltonLength(position.toPermut());
	if(fit < swarm->bestFit) {
		swarm->best = position;
		swarm->bestFit = fit;
		swarm->noChange = 0;
	}
	if(fit < bestFit) {
		best = position;
		bestFit = fit;
	}

	// Zmiana polozenia i predkosci
	for(unsigned int i=0; i<speed.count(); i++)
		speed[i] = OMEGA*speed[i] + C1*randf(0.0f, best[i] - position[i]) + C2*randf(0.0f, bestGlobal[i] - position[i]);
	for(unsigned int i=0; i<position.count(); i++)
		position[i] += speed[i];
}
