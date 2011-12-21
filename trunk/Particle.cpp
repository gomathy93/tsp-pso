#include "stdafx.h"

#include "Particle.h"
#include "TspSwarm.h"
#include "utils.h"

const float WMH::PSO::Particle::OMEGA = 0.75f;
const float WMH::PSO::Particle::C1 = 1.5f;
const float WMH::PSO::Particle::C2 = 1.5f;

void WMH::PSO::Particle::init(TspSwarm* swarm) {
	this->swarm = swarm;
	position.randomize(swarm->g->V());
}

void WMH::PSO::Particle::update() {
	Position& bestGlobal = swarm->best;
	const Graph* graph = swarm->g;

	float fit = position.cost(graph);
	if(fit < swarm->bestFit || swarm->best.size() == 0) {
		swarm->best = position;
		swarm->bestFit = fit;
		swarm->noChange = 0;
	}
	if(fit < bestFit || best.size() == 0) {
		best = position;
		bestFit = fit;
	}

	// Zmiana polozenia i predkosci
	speed = speed * OMEGA + (best - position) * C1 + (bestGlobal - position) * C2;
	position += speed;
}
