#pragma once

namespace WMH {
	/** Losuje liczbe z przedzialu [0, 1] */
	inline static float randf()	{
		return rand()/static_cast<float>(RAND_MAX);
	}

	/** Losuje liczbe z przedialu [min, max] */
	inline static float randf(float min, float max)	{
		return min + randf()*(max - min);
	}

	static const float MAX_FLOAT = 99999999.9999f;
}; // namespace PSO
