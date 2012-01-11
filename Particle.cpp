#include "stdafx.h"

#include "Particle.h"
#include "TspSwarm.h"
#include "utils.h"

// TODO dodac const
float WMH::PSO::Particle::OMEGA = 1.2f;
float WMH::PSO::Particle::C1 = 0.5f;
float WMH::PSO::Particle::C2 = 0.7f;

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
		// TODO debug
		/*wchar_t debug_str[512];
		wsprintf(debug_str, L"new best %d\r\n", (int)fit);
		OutputDebugStringW(debug_str);*/
	}
	if(fit < bestFit || best.size() == 0) {
		best = position;
		bestFit = fit;

//		std::cout << "best: " << best.cost(graph)<<std::endl;
	//std::cout << "pos: " << position.cost(graph)<<std::endl;
	//std::cout << "bestGlobal: " << bestGlobal.cost(graph)<<std::endl;
	}

	



	// Zmiana polozenia i predkosci
	speed = speed * OMEGA + (best - position) * C1 + (bestGlobal - position) * C2;
	position += speed;


	// TODO: debug
	/*wchar_t debug_str[512];
	wsprintf(debug_str, L"speed %d\r\n", speed.size());
	OutputDebugStringW(debug_str);*/
}
