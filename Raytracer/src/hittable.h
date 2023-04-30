// /*
//  * hittable.h
//  */

#pragma once

#include "RTmath/numeric.h"
#include "RTmath/vec3.h"
#include "ray.h"

class material;

struct Hit_Record
{
	point3 p = vec3(0.0f, 0.0f, 0.0f);
	vec3 normal = vec3(0.0f, 0.0f, 0.0f);
	shared_ptr<material> mat_ptr;
	float t = 0.0f;
	bool front_face = true;
	
	inline void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, Hit_Record& record) const = 0;

	
};
