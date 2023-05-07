// /*
//  * vec3.h
//  */

#pragma once

#include <cmath>
#include <iostream>

#include "numeric.h"
#include "bitmap/bitmap_image.hpp"

class Vec3 {
public:
	Vec3(): e{0.0f, 0.0f, 0.0f} {}
	Vec3(const float e0, const float e1, const float e2) : e{e0, e1, e2} {}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }
	uint8_t r() const { return float_to_byte(clamp(x(), 0.0f, 1.0f)); }
	uint8_t g() const { return float_to_byte(clamp(y(), 0.0f, 1.0f)); }
	uint8_t b() const { return float_to_byte(clamp(z(), 0.0f, 1.0f)); }

	Vec3 operator-() const
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

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vec3& operator*=(const float t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vec3& operator/=(const float t)
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

	static Vec3 random()
	{
		return {random_float(), random_float(), random_float()};
	}

	static Vec3 random(const float min, const float max)
	{
		return {random_float(min, max), random_float(min, max), random_float(min, max)};
	}

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		constexpr auto s = 1e-8f;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	float e[3];
};

// Type aliases for vec3
using Point3 = Vec3; // 3D point
using Pos3 = Vec3;   // 3D position
using Color3 = Vec3; // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(float t, const Vec3& v)
{
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, float t)
{
	return t * v;
}

inline Vec3 operator/(Vec3 v, float t)
{
	return (1.0f / t) * v;
}

inline float dot(const Vec3& u, const Vec3& v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return {
		u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]
	};
}

inline Vec3 unit_vector(Vec3 v)
{
	return v / v.length();
}

inline Vec3 get_normal(Vec3 v)
{
	return unit_vector(v);
}

Vec3 random_in_unit_disk();

Vec3 reflect(const Vec3& vec, const Vec3& normal);

Vec3 refract(const Vec3& uv, const Vec3& normal, float etaI_over_etaT);

Vec3 random_in_unit_sphere();

Vec3 random_unit_vector();

Vec3 random_in_hemisphere(const Vec3& normal);

rgb_t get_color(const Color3 pixel_color, const int samples_per_pixel);

// Convert separate R, G, B values to rgb_t struct
rgb_t rgb_to_rgb_t(const unsigned char r, const unsigned char g, const unsigned char b);

rgb_t color_to_rgb_t(const Color3& color);
