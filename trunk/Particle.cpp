#include "stdafx.h"

#include "Particle.h"
#include "TspSwarm.h"
#include "utils.h"

// TODO dodac const
float WMH::PSO::Particle::OMEGA = DEF_OMEGA;
float WMH::PSO::Particle::C1 = DEF_C1;
float WMH::PSO::Particle::C2 = DEF_C2;
int WMH::PSO::Particle::REHOPE = DEF_REHOPE;

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
		// TODO debug
		//wprintf(L"iter=%d, globalBest=%f, particle=%d\r\n", swarm->iter, fit, id);
	}
	if(fit < bestFit || best.size() == 0) {
		best = position;
		bestFit = fit;
		noChange = 0;
		// TODO debug
		//if(id == 1) wprintf(L"iter=%d, new local best %f\r\n", swarm->iter, fit);
		//std::cout << "best: " << best.cost(graph)<<std::endl;
		//std::cout << "pos: " << position.cost(graph)<<std::endl;
		//std::cout << "bestGlobal: " << bestGlobal.cost(graph)<<std::endl;
	}

	if(noChange < REHOPE) {
		// Zmiana polozenia i predkosci
		speed = speed * OMEGA + (best - position) * C1 + (bestGlobal - position) * C2;
		position += speed;
	} else {
		speed.zero();
		position.randomize(graph->V());
		noChange = 0;
	}

	// TODO: debug
	/*wchar_t debug_str[512];
	wsprintf(debug_str, L"speed %d\r\n", speed.size());
	OutputDebugStringW(debug_str);*/
}
