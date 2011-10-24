#include "stdafx.h"

#include "Graph.h"

void PSO::Graph::allocateMatrix(int V) {
	vertexCount = V;
	adjacencyMatrix = new float*[V];
	for (int i = 0; i < vertexCount; i++) {
		adjacencyMatrix[i] = new float[vertexCount];
		for (int j = 0; j < vertexCount; j++)
			adjacencyMatrix[i][j] = 0.0f;
	}
}

PSO::Graph::Graph(int V, int E, float maxDist) {
	allocateMatrix(V);

	for(int i=0; i<E; i++) {
		while(1) {
			int u = rand()%vertexCount;
			int v = rand()%vertexCount;
			if(v == u) continue;
			if(isEdge(u, v)) continue;
			addEdge(u, v, randf(0.1f, maxDist));
			break;
		}
	}
}

float PSO::Graph::hamiltonLength(std::vector<int>& cycle) {
	if(cycle.size() != vertexCount)	return MAX_FLOAT;

	float dist = 0.0f;
	for(int i=0; i<vertexCount-1; i++)
		dist += adjacencyMatrix[cycle[i]][cycle[i+1]];
	dist += adjacencyMatrix[cycle[vertexCount-1]][cycle[0]];
	return dist;
}
