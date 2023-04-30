#include "vec3.h"

vec3 refract(const vec3& uv, const vec3& normal, float etaI_over_etaT)
{
	auto cos_theta = fmin(dot(-uv, normal), 1.0f);
	vec3 R_perp = etaI_over_etaT * (uv + cos_theta * normal);
	vec3 R_parallel = normal * -sqrtf(fabs(1.0f - R_perp.length2()));
	return R_perp + R_parallel;
}

vec3 reflect(const vec3& vec, const vec3& normal)
{
	return vec - 2*dot(vec, normal)*normal;
}

rgb_t get_color(const color3 pixel_color, const int samples_per_pixel)
{
	float r = pixel_color.x();
	float g = pixel_color.y();
	float b = pixel_color.z();


	// Divide the color by the number of samples and gamma-correct for gamma=2.0
	const auto scale = 1.0f / static_cast<float>(samples_per_pixel);
	r = sqrtf(scale * r);
	g = sqrtf(scale * g);
	b = sqrtf(scale * b);
	const uint8_t r_byte = float_to_byte(clamp(r, 0.0f, 1.0f));
	const uint8_t g_byte = float_to_byte(clamp(g, 0.0f, 1.0f));
	const uint8_t b_byte = float_to_byte(clamp(b, 0.0f, 1.0f));
	return { r_byte, g_byte, b_byte };
}

vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = vec3::random(-1, 1);
		if (p.length2() >= 1.0f) continue;
		return p;
	}
}

vec3 random_in_unit_disk()
{
	while (true)
		{
		auto p = vec3(random_float(-1,1), random_float(-1,1), 0);
		if (p.length2() >= 1.0f) continue;
		return p;
	}
}

vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3& normal)
{
	const vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0f)
	{
		// In the same hemisphere as the normal
		return in_unit_sphere;
	}
	
	return -in_unit_sphere;
}

// Convert separate R, G, B values to rgb_t struct
rgb_t rgb_to_rgb_t(const unsigned char r, const unsigned char g, const unsigned char b)
{
	rgb_t rgb;
	rgb.red = r;
	rgb.green = g;
	rgb.blue = b;
	return rgb;
}

rgb_t color_to_rgb_t(const color3& color)
{
	return rgb_to_rgb_t(color.r(), color.g(), color.b());
}