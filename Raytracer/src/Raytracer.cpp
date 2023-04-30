
#include <cstdio>
#include "bitmap/bitmap_image.hpp"

#include "RTmath/numeric.h"
#include "hittables.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"


// Image
constexpr float aspect_ratio = 3.0f / 2.0f;
constexpr int image_width = 600;
constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
constexpr int samples_per_pixel = 50;
constexpr int max_depth = 15;

// Camera
constexpr float viewport_height = 2.0f;
constexpr float viewport_width = aspect_ratio * viewport_height;
constexpr float focal_length = 1.0f;

const auto origin = point3(0, 0, 0);
const auto horizontal = vec3(viewport_width, 0, 0);
const auto vertical = vec3(0, viewport_height, 0);

// Lower left corner is (0,0)
const vec3 lower_left_corner = origin - (horizontal/2.0f) - (vertical/2.0f) - vec3(0,0,focal_length);

float hit_sphere(const point3& center, const float radius, const ray& r)
{
    const vec3 oc               = r.origin() - center;
    const auto a            = r.direction().length2();
    const auto b_half       = dot(oc, r.direction());
    const auto c            = oc.length2() - radius*radius;
    const auto discriminant = b_half*b_half - a*c;
    
    if (discriminant < 0)
    {
        return -1.0f;
    }
    return (-b_half - sqrtf(discriminant) ) / a;
}

color3 ray_color(const ray& r, const hittables& world, int depth)
{
    Hit_Record record;

    if (depth <= 0)
    {
        return {0.0f, 0.0f, 0.0f};
    }
    
    // Sphere hit if true
    if (world.hit(r, 0.001f, infinity, record))
    {
        ray scattered;
        color3 attenuation;

        if (record.mat_ptr->scatter(r, record, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color3(0,0,0);
       // const point3 target = record.p + random_in_hemisphere(record.normal);
       // return 0.5f * ray_color(ray(record.p, target - record.p), world, depth-1);
    }
    
    // Normalize ray direction
    const vec3 unit_direction = get_normal(r.direction());

    // Convert (-1 to 1) y part of vector to (0 to 1)
    const auto t = 0.5f * (unit_direction.y() + 1.0f);

    // blendedValue = (1 - t)*startValue + t*endValue
    // When y=max: 0*startValue + 1*endValue
    // When y=min: 1*startValue + 0*endValue
    // Kind of like blending src & dst in openGL

    // When y is min, set color to white, when at max set to blue
    return (1.0f-t) * color3(1.0f, 1.0f, 1.0f) + t*color3(0.4f, 0.6f, 1.0f);
}

hittables random_scene() {
    hittables world;

    auto ground_material = make_shared<lambertian>(color3(0.5f, 0.5f, 0.5f));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9f*random_float(), 0.2f, b + 0.9f*random_float());

            if ((center - point3(4, 0.2f, 0)).length() > 0.9f) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = random_float(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5f);
                    world.add(make_shared<sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color3(0.4f, 0.2f, 0.1f));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color3(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0f, material3));

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
    
    point3 lookfrom(13, 2, 3);
    point3 lookat(0,0, 0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0f;
    auto aperture = 0.1f;
    

    // Camera
    camera cam (lookfrom, lookat, vup, 25.0f, aspect_ratio, aperture, dist_to_focus);
    
    // Image

    bitmap_image image(image_width, image_height);

    image.set_all_channels(255, 150, 50);

    for (int y = 0; y < image_height; y++)
    {
            std::cerr << "\rScanlines remaining: " << image_height - y << " " << std::flush;
        for (int x = 0; x < image_width; x++)
        {
            color3 pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (static_cast<float>(x) + random_float()) / (image_width - 1);
                auto v = (static_cast<float>(y) + random_float()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
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