#include "constant/constant.h"
#include "tool/color.h"
#include "hit/hittable_list.h"
#include "camera/camera.h"
#include "material/material.h"
#include "shader/shader.h"
#include "scene/scene.h"

#include <iostream>
#include <fstream>


int main() {
    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 1080;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 1;
    const int depth = 0;

    shared_ptr<hittable> lights =
            make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>());

    // World
    scene scene;
    auto world =  scene.scene_generate();

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    color background = color(0, 0, 0);


    std::ofstream ofs;
    //ofs.open("CornellBoxWithSphere.ppm");
    //ofs.open("imageCornellBox.ppm");
    //ofs.open("imageCornellSphere");
    ofs.open("testPathTracing.ppm");

    switch (5) {
        case 1:
            world = scene.random_scene();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2:
            world = scene.simple_light();
            samples_per_pixel = 400;
            background = color(0, 0, 0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
            break;

        default:
        case 5:
            world = scene.cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 10;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
    }

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render

    ofs << "P3\n" << image_width << " " << image_height << "\n255\n";


#pragma omp parallel for schedule(dynamic, 200) private(r)
    for (int j = image_height - 1; j >= 0; --j) {
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples_per_pixel, 100. * (image_height - j) / (image_height));
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for(int s = 0;s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);

                shader shader;
                pixel_color += shader.pathTracing(r, world, depth, background, lights);
            }
            write_color(ofs, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    ofs.close();
    return 0;
}
