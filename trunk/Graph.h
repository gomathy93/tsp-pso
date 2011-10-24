#pragma once

#include "stdafx.h"

#include "Features.h"

namespace PSO {
	/** Klasa reprezentuje graf, wybrano reprezentacje macierzowa */
	class Graph {
		/** Macierz sasiedztwa */
		float** adjacencyMatrix;
		/** Ilosc wierzcholkow */
		int vertexCount;
		/** Konstruktor bezparametrowy - zablokowany, nie tworzymy pustych grafow */
		Graph() {
			adjacencyMatrix = new float*[0];
			vertexCount = 0;
		}

		/** Alokuje pamiec na V wierzcholkow */
		void allocateMatrix(int V);
	public:
		/** Tworzy graf o V wierzcholkach */
		Graph(int V) {
			allocateMatrix(V);
		}

		/** Tworzy graf z V wierzcholkami i E krawedziami o dlugosciach z zakresu (0.1, maxDist) */
		Graph(int V, int E, float maxDist);

		/** Destruktor */
		~Graph() {
			for (int i = 0; i < vertexCount; i++)
				delete[] adjacencyMatrix[i];
			delete[] adjacencyMatrix;
		}

		/** Dodaje krawedz i-j o dlugosci dist */
		void addEdge(int i, int j, float dist) {
			if (i >= 0 && i < vertexCount && j > 0 && j < vertexCount) {
				adjacencyMatrix[i][j] = dist;
				adjacencyMatrix[j][i] = dist;
			}
		}

		/** Usuwa krawedz i-j */
		void removeEdge(int i, int j) {
			if (i >= 0 && i < vertexCount && j > 0 && j < vertexCount) {
				adjacencyMatrix[i][j] = 0.0f;
				adjacencyMatrix[j][i] = 0.0f;
			}
		}

		/** Sprawdza czy istnieje krawedz i-j */
		bool isEdge(int i, int j) { 
			if (i >= 0 && i < vertexCount && j > 0 && j < vertexCount)
				return (adjacencyMatrix[i][j] > 0.0f);
			else
				return false;
		}

		/** Zwraca liczbe wierzcholkow */
		int V()
		{
			return vertexCount;
		}

		/** Wylicza dlugosc cyklu Hamiltona */
		float hamiltonLength(std::vector<int>& cycle);
	};
}; // namespace PSO
