#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"
using namespace PSO;

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	Graph g(100, 4950, 10.0f); // pelny
	TspSwarm s(&g, 50);
	s.compute();
	return 0;
}

