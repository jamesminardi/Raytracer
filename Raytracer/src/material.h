// /*
//  * material.h
//  */

#pragma once

#include "RTmath/numeric.h"
#include "ray.h"
#include "RTmath/vec3.h"
#include "hittable.h"

struct Hit_Record;

class material
{
public:
	virtual bool scatter(const ray& ray_in, const Hit_Record& record, color3& attenuation, ray& scattered) const = 0;
	
};


class lambertian : public material
{
public:
	lambertian(const color3& color) : albedo(color) {}

	virtual bool scatter(const ray& ray_in, const Hit_Record& record, color3& attenuation, ray& scattered) const override
	{
		auto scatter_direction = record.normal + random_unit_vector();

		if (scatter_direction.near_zero())
		{
			scatter_direction = record.normal;
		}
		
		scattered = ray(record.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

	color3 albedo;
};

class metal : public material
{
public:
	metal(const color3& color, const float fuzziness) : albedo(color), fuzz(fuzziness){}

	virtual bool scatter(const ray& ray_in, const Hit_Record& record, color3& attenuation, ray& scattered) const override
	{
		const vec3 reflected = reflect(unit_vector(ray_in.direction()), record.normal);
		scattered = ray(record.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), record.normal) > 0);
	}

	color3 albedo;
	float fuzz;
};

class dielectric : public material
{
public:
	dielectric(const float index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(const ray& ray_in, const Hit_Record& record, color3& attenuation, ray& scattered) const override
	{
		attenuation                  = color3(1.0f, 1.0f, 1.0f);
		const float refraction_ratio = record.front_face ? (1.0f/ir) : ir;
		const vec3 unit_direction    = unit_vector(ray_in.direction());

		const float cos_theta = fmin(dot(-unit_direction, record.normal), 1.0f);
		const float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

		const bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
		{
			direction = reflect(unit_direction, record.normal);
		}
		else
		{
			direction = refract(unit_direction, record.normal, refraction_ratio);
		}

		scattered = ray(record.p, direction);
		
		return true;
	}

	float ir; // index of refraction

private:
	static float reflectance(float cosine, float ref_idx)
	{
		// Schlick's approximation for reflectance
		auto r0 = (1-ref_idx) / (1+ref_idx);
		r0 *= r0;
		return r0 + (1-r0) * powf((1 - cosine), 5);
	}
};