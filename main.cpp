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

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    // World
    scene scene;
    auto world =  scene.random_scene();

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    std::ofstream ofs;
    ofs.open("image2.ppm");

    // Render

    ofs << "P3\n" << image_width << " " << image_height << "\n255\n";


#pragma omp parallel for schedule(dynamic, 8) private(r)
    for (int j = image_height - 1; j >= 0; --j) {
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samples_per_pixel, 100. * (image_height - j) / (image_height));
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for(int s = 0;s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);

                shader shader;
                pixel_color += shader.render(r, world, max_depth);
            }
            write_color(ofs, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    ofs.close();
    return 0;
}
