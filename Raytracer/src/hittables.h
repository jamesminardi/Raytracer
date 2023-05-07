// /*
//  * hittables.h
//  */

#pragma once

#include "hittable.h"

#include <vector>
#include "ray.h"
#include "RTmath/numeric.h"
#include "RTmath/vec3.h"


using std::shared_ptr;
using std::make_shared;

class Hittables : public Hittable {
public:
	Hittables() = default;
	explicit Hittables(shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<Hittable> object) { objects.push_back(object); }

	bool hit(const Ray& r, float t_min, float t_max, Hit_Record& record) const override;

	std::vector<shared_ptr<Hittable>> objects;
};
