#pragma once

#include <iostream>
#include <vector>
using namespace std;

float randf()
{
	return rand()/static_cast<float>(RAND_MAX);
}

float randf(float min, float max)
{
	return min + randf()*(max - min);
}
