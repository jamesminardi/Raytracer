// /*
//  * vec3.h
//  */

#pragma once

#include <cmath>
#include <iostream>

#include "numeric.h"
#include "bitmap_image.hpp"

class Vec3 {
public:

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
	Vec3(): x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(const float e0, const float e1, const float e2) : x(e0), y(e1), z(e2) {}
#pragma clang diagnostic pop

//	float x() const { return x; }
//	float y() const { return e[1]; }
//	float z() const { return e[2]; }
//	uint8_t r() const { return float_to_byte(clamp(x(), 0.0f, 1.0f)); }
//	uint8_t g() const { return float_to_byte(clamp(y(), 0.0f, 1.0f)); }
//	uint8_t b() const { return float_to_byte(clamp(z(), 0.0f, 1.0f)); }

	Vec3 operator-() const
	{
		return {-x, -y, -z};
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
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vec3& operator*=(const float t)
	{
		x *= t;
		y *= t;
		z *= t;
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
		return x * x + y * y + z * z;
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
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}

	union {
		struct {
			float x, y, z;
		};
		struct {
			float r{}, g{}, b{};
		};
		float e[3];
	};
};

// Type aliases for vec3
using Point3 = Vec3; // 3D point
using Pos3 = Vec3;   // 3D position
using Color3 = Vec3; // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return {u.x + v.x, u.y + v.y, u.z + v.z};
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return {u.x - v.x, u.y - v.y, u.z - v.z};
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return {u.x * v.x, u.y * v.y, u.z * v.z};
}

inline Vec3 operator*(float t, const Vec3& v)
{
	return {t * v.x, t * v.y, t * v.z};
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
	return u.x * v.x
		+ u.y * v.y
		+ u.z * v.z;
}

inline Vec3 cross(const Vec3& u, const Vec3& v)
{
	return {
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
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

rgb_t get_color(Color3 pixel_color, int samples_per_pixel);

// Convert separate R, G, B values to rgb_t struct
rgb_t rgb_to_rgb_t(unsigned char r, unsigned char g, unsigned char b);

rgb_t color_to_rgb_t(const Color3& color);
