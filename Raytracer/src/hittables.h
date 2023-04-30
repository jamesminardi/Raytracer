// /*
//  * hittables.h
//  */

#pragma once

#include "hittable.h"

#include "RTmath/numeric.h"
#include <vector>
#include "RTmath/vec3.h"
#include "ray.h"


using std::shared_ptr;
using std::make_shared;

class hittables : public hittable
{
public:
	hittables() = default;
	hittables(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, float t_min, float t_max, Hit_Record& record) const override;

	std::vector<shared_ptr<hittable>> objects;
	
};