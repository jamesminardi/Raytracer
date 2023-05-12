cmake_minimum_required(VERSION 3.25)
project(Raytracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(Raytracer/src)
include_directories(Raytracer/src/math)


add_executable(Raytracer
        Raytracer/src/math/numeric.cpp
        Raytracer/src/math/numeric.h
        Raytracer/src/math/vec3.cpp
        Raytracer/src/math/vec3.h
        Raytracer/src/camera.cpp
        Raytracer/src/camera.h
        Raytracer/src/hittable.cpp
        Raytracer/src/hittable.h
        Raytracer/src/hittables.cpp
        Raytracer/src/hittables.h
        Raytracer/src/material.cpp
        Raytracer/src/material.h
        Raytracer/src/ray.cpp
        Raytracer/src/ray.h
        Raytracer/src/raytracer.cpp
        Raytracer/src/sphere.cpp
        Raytracer/src/sphere.h
        Raytracer/Raytracer.vcxproj
        Raytracer/Raytracer.vcxproj.filters)


target_include_directories (Raytracer PUBLIC includes/)