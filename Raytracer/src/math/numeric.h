// /*
//  * numeric.h
//  */

#pragma once

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

// Using

using std::shared_ptr;
using std::make_shared;
using std::fabs;

// Constants

const float infinity = std::numeric_limits<float>::infinity();
constexpr float pi   = 3.1415926535897932385f;


// Utility functions

// Template function to convert float or double with a range of 0.0 through 1.0 to a byte with a range of 0 through 255 with rounding
template <typename T>
uint8_t float_to_byte(T value)
{
	return static_cast<uint8_t>(std::round(value * 255.0));
}

inline float degrees_to_radians(const float degrees)
{
	return degrees * pi / 180.0f;
}

inline float clamp(const float x, const float min, const float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Random Utilities

// Returns a random real in [0, 1)
inline float random_float()
{
	return static_cast<float>(rand()) / (RAND_MAX + 1.0f);
}


// Returns a random real in [min, max)
inline float random_float(const float min, const float max)
{
	return min + (max - min) * random_float();
}
