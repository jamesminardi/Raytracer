// /*
//  * camera.h
//  */

#pragma once

#include "ray.h"
#include "RTmath/numeric.h"
#include "RTmath/vec3.h"

class Camera {
public:
	Camera(const Point3 lookfrom, const Point3 lookat, const Vec3 vup,
		   const float vfov, const float aspect_ratio, const float aperture, const float focus_dist)
	{
		const auto theta = degrees_to_radians(vfov);
		const auto h     = tanf(theta / 2.0f);

		const auto viewport_height  = 2.0f * h;
		const auto viewport_width   = aspect_ratio * viewport_height;
		constexpr auto focal_length = 1.0f;


		w_ = unit_vector(lookfrom - lookat);
		u_ = unit_vector(cross(vup, w_));
		v_ = cross(w_, u_);

		origin_            = lookfrom;
		horizontal_        = focus_dist * viewport_width * u_;
		vertical_          = focus_dist * viewport_height * v_;
		lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w_;

		lens_radius_ = aperture / 2.0f;
	}

	Ray get_ray(const float s, const float t) const
	{
		const Vec3 rd     = lens_radius_ * random_in_unit_disk();
		const Vec3 offset = u_ * rd.x() + v_ * rd.y();

		return {origin_ + offset, lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset};
	}

private:
	Point3 origin_;
	Point3 lower_left_corner_;
	Vec3 horizontal_;
	Vec3 vertical_;
	Vec3 u_, v_, w_;
	float lens_radius_;
};
