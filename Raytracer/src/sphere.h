// /*
//  * sphere.h
//  */

#pragma once
#include "hittable.h"
#include "ray.h"
#include "RTmath/numeric.h"
#include "RTmath/vec3.h"

class Sphere : public Hittable {
public:
	Sphere() : m_center({0, 0, 0}), m_radius(0) {}

	Sphere(const Point3 center, const float radius, shared_ptr<Material> mat)
		: m_center(center), m_radius(radius), mat_ptr(mat) {}

	bool hit(const Ray& r, float t_min, float t_max, Hit_Record& record) const override;

	Point3 m_center;
	float m_radius;
	shared_ptr<Material> mat_ptr;
};
