cmake_minimum_required(VERSION 3.21)
project(pathTracingDemo)

set(CMAKE_CXX_STANDARD 14)

add_executable(pathTracingDemo main.cpp tool/vec3.h tool/color.h shape/sphere.h ray/ray.h material/material.h camera/camera.h constant/constant.h hit/hittable.h hit/hittable_list.h shader/shader.h scene/scene.h shape/moving_sphere.h accerlate/aabb.h accerlate/bvh.h material/texture.h hit/aarect.h shape/box.h)
