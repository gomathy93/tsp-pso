#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <list>
#include <fstream>
#include <cmath>
#include <cassert>
#ifdef _WIN32
# include <windows.h>
#else
# error "define getTickCount"
#endif

typedef struct AlgoResults {
	int		result;
	int		verticesCount;
	float	psoCost;
	float	saCost;
	float	rsCost;
} AlgoResults;

// ta funkcja bedzie wywolywana przez GUI
extern "C" __declspec(dllexport) AlgoResults __cdecl solveGraph(
		const char* graph, float C1, float C2, 
		float OMEGA, int REHOPE, int NOCHANGE);
