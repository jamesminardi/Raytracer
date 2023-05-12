// /*
//  * sphere.h
//  */

#pragma once
#include "hittable.h"
#include "ray.h"
#include "math/numeric.h"
#include "math/vec3.h"

class Sphere : public Hittable {
public:
	Sphere() : center({0, 0, 0}), radius(0) {}

	Sphere(const Point3 center, const float radius, shared_ptr<Material> mat)
		: center(center), radius(radius), mat_ptr(mat) {}

	bool hit(const Ray& r, float t_min, float t_max, Hit_Record& record) const override;

	Point3 center;
	float radius;
	shared_ptr<Material> mat_ptr;
};
