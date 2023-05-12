// /*
//  * material.h
//  */

#pragma once

#include "hittable.h"
#include "ray.h"
#include "math/numeric.h"
#include "math/vec3.h"

struct Hit_Record;

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& ray_in, const Hit_Record& record, Color3& attenuation, Ray& scattered) const = 0;
};


class Lambertian final : public Material {
public:
	explicit Lambertian(const Color3& color) : albedo(color) {}

	bool scatter(const Ray& ray_in, const Hit_Record& record, Color3& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = record.normal + random_unit_vector();

		if (scatter_direction.near_zero()) {
			scatter_direction = record.normal;
		}

		scattered   = Ray(record.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

	Color3 albedo;
};

class Metal : public Material {
public:
	Metal(const Color3& color, const float fuzziness) : albedo(color), fuzz(fuzziness) {}

	bool scatter(const Ray& ray_in, const Hit_Record& record, Color3& attenuation, Ray& scattered) const override
	{
		const Vec3 reflected = reflect(unit_vector(ray_in.direction()), record.normal);
		scattered            = Ray(record.p, reflected + fuzz * random_in_unit_sphere());
		attenuation          = albedo;
		return (dot(scattered.direction(), record.normal) > 0);
	}

	Color3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	explicit Dielectric(const float index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const Ray& ray_in, const Hit_Record& record, Color3& attenuation, Ray& scattered) const override
	{
		attenuation                  = Color3(1.0f, 1.0f, 1.0f);
		const float refraction_ratio = record.front_face ? (1.0f / ir) : ir;
		const Vec3 unit_direction    = unit_vector(ray_in.direction());
		const float cos_theta        = fmin(dot(-unit_direction, record.normal), 1.0f);
		const float sin_theta        = sqrtf(1.0f - cos_theta * cos_theta);

		const bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
		Vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float()) {
			direction = reflect(unit_direction, record.normal);
		}
		else {
			direction = refract(unit_direction, record.normal, refraction_ratio);
		}

		scattered = Ray(record.p, direction);

		return true;
	}

	float ir; // index of refraction

private:
	static float reflectance(float cosine, float ref_idx)
	{
		// Schlick's approximation for reflectance
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 *= r0;
		return r0 + (1 - r0) * powf((1 - cosine), 5);
	}
};
