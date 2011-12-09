#include "stdafx.h"

#include "Graph.h"
#include "TspSwarm.h"

using namespace WMH;

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	Graph g(5, 10.0f);
	PSO::TspSwarm swarm(&g, 50);
	swarm.compute();
	std::cout.precision(5);
	std::cout << g << std::endl;
	std::cout << swarm << std::endl;
	system("PAUSE");
	return 0;
}
