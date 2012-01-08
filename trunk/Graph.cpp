#include "stdafx.h"

#include "Graph.h"

void WMH::Graph::allocateMatrix(int V) {
	vertexCount = V;
	adjacencyMatrix = new float*[V];
	for (int i = 0; i < vertexCount; i++) {
		adjacencyMatrix[i] = new float[vertexCount];
		for (int j = 0; j < vertexCount; j++)
			adjacencyMatrix[i][j] = 99999.0f;
	}
}

void WMH::Graph::copyMatrix(const Graph& g2) {
	allocateMatrix(g2.V());
	for(int i=0; i<vertexCount; i++)
		for(int j=0; j<vertexCount; j++)
			adjacencyMatrix[i][j] = g2.adjacencyMatrix[i][j];
}

void WMH::Graph::freeMatrix() {
	if(adjacencyMatrix)	{
		for (int i = 0; i < vertexCount; i++)
			delete[] adjacencyMatrix[i];
		delete[] adjacencyMatrix;
	}
}

WMH::Graph::Graph(int V, float maxDist) {
	allocateMatrix(V);

	if(maxDist < 0.1f)
		maxDist = 0.1f;

	for(int i=0; i<V; i++)
		for(int j=i+1; j<V; j++)
			addEdge(i, j, randf(0.1f, maxDist));
}

float WMH::Graph::hamiltonLength(std::vector<int>& cycle) const {
	if(cycle.size() != vertexCount)	return MAX_FLOAT;

	float dist = 0.0f;
	for(int i=0; i<vertexCount-1; i++) {
		if(adjacencyMatrix[cycle[i]][cycle[i+1]] == 0.0f) return MAX_FLOAT; // brak sciezki
		dist += adjacencyMatrix[cycle[i]][cycle[i+1]];
	}
	if(adjacencyMatrix[cycle[vertexCount-1]][cycle[0]] == 0.0f)	return MAX_FLOAT; // brak sciezki
	dist += adjacencyMatrix[cycle[vertexCount-1]][cycle[0]];
	return dist;
}

std::ostream& WMH::operator << (std::ostream& stream, const WMH::Graph& g) {

	stream << g.V() << std::endl;
	for(int i=0; i<g.V(); i++) {
		for(int j=0; j<g.V(); j++) stream << g.getDist(i, j) << '\t';
		stream << std::endl;
	}
	return stream;
}

std::istream& WMH::operator >> (std::istream& stream, WMH::Graph& g) {
	g.freeMatrix();
	stream >> g.vertexCount;
	g.allocateMatrix(g.vertexCount);
	for(int x=0; x<g.vertexCount; x++)
		for(int y=0; y<g.vertexCount; y++)
			stream >> g.adjacencyMatrix[y][x];
	return stream;
}
