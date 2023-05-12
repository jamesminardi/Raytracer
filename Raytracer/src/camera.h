// /*
//  * camera.h
//  */

#pragma once

#include "ray.h"
#include "math/numeric.h"
#include "math/vec3.h"

class Camera {
public:
	Camera(const Point3 look_from, const Point3 look_at, const Vec3 v_up,
           const float v_fov, const float aspect_ratio, const float aperture, const float focus_dist)
	{
		const auto theta = degrees_to_radians(v_fov);
		const auto h     = tanf(theta / 2.0f);

		const auto viewport_height  = 2.0f * h;
		const auto viewport_width   = aspect_ratio * viewport_height;
		constexpr auto focal_length = 1.0f;


        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(v_up, w));
        v = cross(w, u);

        origin            = look_from;
        horizontal        = focus_dist * viewport_width * u;
        vertical          = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2.0f;
	}

	Ray get_ray(const float s, const float t) const
	{
		const Vec3 rd     = lens_radius * random_in_unit_disk();
		const Vec3 offset = u * rd.x + v * rd.y;

		return {origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset};
	}

private:
	Point3 origin;
	Point3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lens_radius;
};
