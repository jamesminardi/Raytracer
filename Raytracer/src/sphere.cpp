﻿#include "sphere.h"



bool sphere::hit(const ray& r, float t_min, float t_max, Hit_Record& record) const
{

	const vec3 oc               = r.origin() - m_center;
	const auto a            = r.direction().length2();
	const auto b_half       = dot(oc, r.direction());
	const auto c            = oc.length2() - m_radius*m_radius;
	const auto discriminant = b_half*b_half - a*c;
    
	if (discriminant < 0) return false;
	const auto sqrtd = sqrtf(discriminant);

	// Find the nearest root that lies in the acceptable range
	auto root = (-b_half - sqrtd) / a;
	if (root < t_min || root > t_max)
	{
		root = (-b_half + sqrtd) / a;
		if (root < t_min || root > t_max)
		{
			return false;
		}
	}

	record.t = root;
	record.p = r.at(record.t);

	// Normal determination
	// Should normal always point outward from the surface or always point against the ray?
	const vec3 outward_normal = (record.p - m_center) / m_radius;
	record.set_face_normal(r, outward_normal);
	record.mat_ptr = mat_ptr;
	return true;
	
}
