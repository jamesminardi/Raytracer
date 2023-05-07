#include <cstdio>
#include "bitmap/bitmap_image.hpp"

#include "camera.h"
#include "hittables.h"
#include "material.h"
#include "sphere.h"
#include "RTmath/numeric.h"


// Image
constexpr float aspect_ratio    = 3.0f / 2.0f;
constexpr int image_width       = 600;
constexpr int image_height      = static_cast<int>(image_width / aspect_ratio);
constexpr int samples_per_pixel = 50;
constexpr int max_depth         = 15;

// Camera
constexpr float viewport_height = 2.0f;
constexpr float viewport_width  = aspect_ratio * viewport_height;
constexpr float focal_length    = 1.0f;

const auto origin     = Point3(0, 0, 0);
const auto horizontal = Vec3(viewport_width, 0, 0);
const auto vertical   = Vec3(0, viewport_height, 0);

// Lower left corner is (0,0)
const Vec3 lower_left_corner = origin - (horizontal / 2.0f) - (vertical / 2.0f) - Vec3(0, 0, focal_length);

float hit_sphere(const Point3& center, const float radius, const Ray& r)
{
	const Vec3 oc           = r.origin() - center;
	const auto a            = r.direction().length2();
	const auto b_half       = dot(oc, r.direction());
	const auto c            = oc.length2() - radius * radius;
	const auto discriminant = b_half * b_half - a * c;

	if (discriminant < 0) {
		return -1.0f;
	}
	return (-b_half - sqrtf(discriminant)) / a;
}

Color3 ray_color(const Ray& r, const Hittables& world, int depth)
{
	Hit_Record record;

	if (depth <= 0) {
		return {0.0f, 0.0f, 0.0f};
	}

	// Sphere hit if true
	if (world.hit(r, 0.001f, infinity, record)) {
		Ray scattered;
		Color3 attenuation;

		if (record.mat_ptr->scatter(r, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return {0, 0, 0};
		// const point3 target = record.p + random_in_hemisphere(record.normal);
		// return 0.5f * ray_color(ray(record.p, target - record.p), world, depth-1);
	}

	// Normalize ray direction
	const Vec3 unit_direction = get_normal(r.direction());

	// Convert (-1 to 1) y part of vector to (0 to 1)
	const auto t = 0.5f * (unit_direction.y() + 1.0f);

	// blendedValue = (1 - t)*startValue + t*endValue
	// When y=max: 0*startValue + 1*endValue
	// When y=min: 1*startValue + 0*endValue
	// Kind of like blending src & dst in openGL

	// When y is min, set color to white, when at max set to blue
	return (1.0f - t) * Color3(1.0f, 1.0f, 1.0f) + t * Color3(0.4f, 0.6f, 1.0f);
}

Hittables random_scene()
{
	Hittables world;

	auto ground_material = make_shared<Lambertian>(Color3(0.5f, 0.5f, 0.5f));
	world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			const auto choose_mat = random_float();
			Point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if ((center - Point3(4, 0.2f, 0)).length() > 0.9f) {
				shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo     = Color3::random() * Color3::random();
					sphere_material = make_shared<Lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95f) {
					// metal
					auto albedo     = Color3::random(0.5, 1);
					auto fuzz       = random_float(0, 0.5);
					sphere_material = make_shared<Metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<Dielectric>(1.5f);
					world.add(make_shared<Sphere>(center, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color3(0.4f, 0.2f, 0.1f));
	world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color3(0.7f, 0.6f, 0.5f), 0.0f);
	world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0f, material3));

	return world;
}

int main(int argc, char* argv[])
{
	// World
	//hittables world;
	/*
		auto material_ground = make_shared<lambertian>(color3(0.8f, 0.8f, 0.0f));
		auto material_center = make_shared<lambertian>(color3(0.1f, 0.2f, 0.5f));
		auto material_left = make_shared<dielectric>(1.5f);
		auto material_right = make_shared<metal>(color3(0.8f,0.6f,0.4f), 0.5f);
		
		world.add(make_shared<sphere>(point3( 0.0f, -100.5f, -1.0f), 100.0f, material_ground));
		world.add(make_shared<sphere>(point3( 0.0f,    0.0f, -1.0f),   0.5f, material_center));
		world.add(make_shared<sphere>(point3(-1.0f,    0.0f, -1.0f),   0.5f, material_left));
		world.add(make_shared<sphere>(point3(-1.0f,    0.0f, -1.0f), -0.45f, material_left));
		world.add(make_shared<sphere>(point3( 1.0f,    0.0f, -1.0f),   0.5f, material_right));
	*/

	auto world = random_scene();

	const Point3 look_from(13, 2, 3);
	const Point3 look_at(0, 0, 0);
	const Vec3 vup(0, 1, 0);
	constexpr auto dist_to_focus = 10.0f;
	constexpr auto aperture      = 0.1f;


	// Camera
	Camera cam(look_from, look_at, vup, 25.0f, aspect_ratio, aperture, dist_to_focus);

	// Image

	bitmap_image image(image_width, image_height);

	image.set_all_channels(255, 150, 50);

	for (int y = 0; y < image_height; y++) {
		std::cerr << "\rScanlines remaining: " << image_height - y << " " << std::flush;
		for (int x = 0; x < image_width; x++) {
			Color3 pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) {
				const auto u = (static_cast<float>(x) + random_float()) / (image_width - 1);
				const auto v = (static_cast<float>(y) + random_float()) / (image_height - 1);
				Ray r        = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			//color3 color(static_cast<float>(x) / (image_width - 1), static_cast<float>(y) / (image_height - 1), 0.25f);

			//const auto u = static_cast<float>(x) / (image_width - 1);
			//const auto v = static_cast<float>(y) / (image_height - 1);
			//ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
			//color3 pixel_color = ray_color(r, world);

			image.set_pixel(x, y, get_color(pixel_color, samples_per_pixel));
		}
	}

	// Render

	image.vertical_flip();

	image.save_image("output.bmp");

	return 0;
}
