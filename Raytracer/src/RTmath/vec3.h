﻿// /*
//  * vec3.h
//  */

#pragma once

#include <cmath>
#include <iostream>

#include "numeric.h"
#include "bitmap/bitmap_image.hpp"

class vec3
{
public:
	vec3(): e{0.0f,0.0f,0.0f} {}
	vec3(const float e0, const float e1, const float e2) : e{e0, e1, e2} {}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }
	uint8_t r() const { return float_to_byte(clamp(x(), 0.0f, 1.0f)); }
	uint8_t g() const { return float_to_byte(clamp(y(), 0.0f, 1.0f)); }
	uint8_t b() const { return float_to_byte(clamp(z(), 0.0f, 1.0f)); }
	
	vec3 operator-() const
	{
		return {-e[0], -e[1], -e[2]};
	}
	
	float operator[](int i) const
	{
		return e[i];
	}

	float& operator[](int i)
	{
		return e[i];
	}

	vec3& operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const float t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const float t)
	{
		return *this *= 1.0f / t;
	}

	float length() const
	{
		return std::sqrt(length2());
	}

	float length2() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	static vec3 random()
	{
		return { random_float(), random_float(), random_float() };
	}

	static vec3 random(const float min, const float max)
	{
		return { random_float(min, max), random_float(min, max), random_float(min, max) };
	}

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		constexpr auto s = 1e-8f;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	float e[3];
	
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using pos3 = vec3; // 3D position
using color3 = vec3; // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3& v)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, float t)
{
	return t * v;
}

inline vec3 operator/(vec3 v, float t)
{
	return (1.0f / t) * v;
}

inline float dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

inline vec3 get_normal(vec3 v)
{
	return unit_vector(v);
}

vec3 random_in_unit_disk();

vec3 reflect(const vec3& vec, const vec3& normal);

vec3 refract(const vec3& uv, const vec3& normal, float etaI_over_etaT);

vec3 random_in_unit_sphere();

vec3 random_unit_vector();

vec3 random_in_hemisphere(const vec3& normal);

rgb_t get_color(const color3 pixel_color, const int samples_per_pixel);

// Convert separate R, G, B values to rgb_t struct
rgb_t rgb_to_rgb_t(const unsigned char r, const unsigned char g, const unsigned char b);

rgb_t color_to_rgb_t(const color3& color);