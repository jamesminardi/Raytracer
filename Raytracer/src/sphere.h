// /*
//  * sphere.h
//  */

#pragma once
#include "hittable.h"
#include "RTmath/numeric.h"
#include "RTmath/vec3.h"
#include "ray.h"

class sphere : public hittable
{
public:
	sphere() : m_center({0,0,0}), m_radius(0) {}
	sphere (const point3 center, const float radius, shared_ptr<material> mat)
		: m_center(center), m_radius(radius), mat_ptr(mat)  {}

	virtual bool hit(const ray& r, float t_min, float t_max, Hit_Record& record) const override;
	
	point3 m_center;
	float m_radius;
	shared_ptr<material> mat_ptr;
};
