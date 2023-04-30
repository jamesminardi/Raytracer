#include "hittables.h"


bool hittables::hit(const ray& r, const float t_min, const float t_max, Hit_Record& record) const
{
	Hit_Record temp_record;

	bool hit_anything = false;

	auto current_closest = t_max;

	for (const auto& object : objects)
	{
		if (object->hit(r, t_min, current_closest, temp_record))
		{
			hit_anything = true;
			current_closest = temp_record.t;
			record = temp_record;
		}
	}
		return hit_anything;
}