// /*
//  * hittable.h
//  */

#pragma once

#include "ray.h"
#include "RTmath/numeric.h"
#include "RTmath/vec3.h"

class Material;

struct Hit_Record {
	Point3 p    = Vec3(0.0f, 0.0f, 0.0f);
	Vec3 normal = Vec3(0.0f, 0.0f, 0.0f);
	shared_ptr<Material> mat_ptr;
	float t         = 0.0f;
	bool front_face = true;

	inline void set_face_normal(const Ray& r, const Vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal     = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& record) const = 0;
};
