// /*
//  * ray.h
//  */

#pragma once

#include "RTmath/numeric.h"
#include "RTmath/vec3.h"

class Ray {
public:
	Ray() = default;
	Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

	Point3 origin() const { return orig; }
	Vec3 direction() const { return dir; }

	Point3 at(const float time) const
	{
		return orig + (time * dir);
	}

	Point3 orig;
	Vec3 dir;
};
