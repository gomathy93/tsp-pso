#pragma once

#include "stdafx.h"

#include "Features.h"

namespace WMH {
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

		/** Przypisuje graf g2, usun wczesniejsza macierz! */
		void copyMatrix(const Graph& g2);

		/** Zwalnia pamiec na macierz */
		void freeMatrix();
	public:
		/** Tworzy graf o V wierzcholkach */
		Graph(int V) {
			allocateMatrix(V);
		}

		/** Tworzy graf pelny z V wierzcholkami o dlugosciach z zakresu (0.1, maxDist) */
		Graph(int V, float maxDist);

		/** To musi byc zadeklarowane zeby uniknac glupich bledow */
		Graph(const Graph& g2) {
			copyMatrix(g2);
		}

		/** Wczytuje graf z pliku */
		Graph(const char* filename) {
			std::ifstream fin(filename);
			vertexCount = 0;
			adjacencyMatrix = NULL;
			if(fin.good()) {
				fin >> *this;
				fin.close();
			}
		}

		/** To musi byc zadeklarowane zeby uniknac glupich bledow */
		Graph& operator = (const Graph& g2)	{
			if (this == &g2) 
				return *this;
			freeMatrix();
			copyMatrix(g2);
			return *this;
		} 

		/** Destruktor */
		~Graph() {
			freeMatrix();
		}

		/** Dodaje krawedz i-j o dlugosci dist */
		inline void addEdge(int i, int j, float dist) {
			if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount) {
				adjacencyMatrix[i][j] = dist;
				adjacencyMatrix[j][i] = dist;
			}
		}

		/** Usuwa krawedz i-j */
		inline void removeEdge(int i, int j) {
			if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount) {
				adjacencyMatrix[i][j] = 0.0f;
				adjacencyMatrix[j][i] = 0.0f;
			}
		}

		/** Sprawdza czy istnieje krawedz i-j */
		inline bool isEdge(int i, int j) const { 
			if (i >= 0 && i < vertexCount && j >= 0 && j < vertexCount)
				return (adjacencyMatrix[i][j] > 0.0f);
			else
				return false;
		}

		/** Zwraca odleglosc pomiedzy wierzcholkami i-j */
		inline float getDist(int i, int j) const {
			if(isEdge(i, j))
				return adjacencyMatrix[i][j];
			else
				return 0.0f;
		}

		/** Zwraca liczbe wierzcholkow */
		inline int V() const {
			return vertexCount;
		}

		/** Wylicza dlugosc cyklu Hamiltona */
		float hamiltonLength(std::vector<int>& cycle) const;

		/** Zapisuje graf do strumienia */
		friend std::ostream& operator << (std::ostream& stream, const WMH::Graph& g);

		/** Wczytuje graf ze strumienia */
		friend std::istream& operator >> (std::istream& stream, WMH::Graph& g) {
			g.freeMatrix();
			stream >> g.vertexCount;
			g.allocateMatrix(g.vertexCount);
			for(int x=0; x<g.vertexCount; x++)
				for(int y=0; y<g.vertexCount; y++)
					stream >> g.adjacencyMatrix[y][x];
			return stream;
		}
	};
}; // namespace WMH
