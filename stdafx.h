#pragma once

#include <iostream>
#include <vector>
using namespace std;

/** Losuje liczbe z przedzialu [0, 1] */
inline static float randf()
{
	return rand()/static_cast<float>(RAND_MAX);
}

/** Losuje liczbe z przedialu [min, max] */
inline static float randf(float min, float max)
{
	return min + randf()*(max - min);
}
