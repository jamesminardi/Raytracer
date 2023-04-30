// /*
//  * ray.h
//  */

#pragma once

#include "RTmath/numeric.h"
#include "RTmath/vec3.h"

class ray
{
public:
	ray() = default;
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	point3 at(const float time) const
	{
		return orig + (time*dir);
	}
	
	point3 orig;
	vec3 dir;
	
};
