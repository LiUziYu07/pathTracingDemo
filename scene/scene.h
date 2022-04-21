//
// Created by lzy on 2022/4/18.
//

#ifndef PATHTRACINGDEMO_SCENE_H
#define PATHTRACINGDEMO_SCENE_H

#include "../material/material.h"
#include "../hit/hittable_list.h"

class scene{

public:
    static hittable_list scene_generate();
    inline hittable_list random_scene() {
        hittable_list world;

        auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
        world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = random_double();
                point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

                if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                    shared_ptr<material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = color::random() * color::random();
                        sphere_material = make_shared<lambertian>(albedo);
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    } else if (choose_mat < 0.95) {
                        // metal
                        auto albedo = color::random(0.5, 1);
                        auto fuzz = random_double(0, 0.5);
                        sphere_material = make_shared<metal>(albedo, fuzz);
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    } else {
                        // glass
                        sphere_material = make_shared<dielectric>(1.5);
                        world.add(make_shared<sphere>(center, 0.2, sphere_material));
                    }
                }
            }
        }

        auto material1 = make_shared<dielectric>(1.5);
        world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
        world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

        return world;
    }
};

hittable_list scene::scene_generate() {
    hittable_list world;

    auto left_sphere = make_shared<lambertian>(color(0.75,0.25,0.25));
    auto right_sphere = make_shared<lambertian>(color(0.25,0.25,0.75));
    auto back_sphere = make_shared<lambertian>(color(0.75,0.75,0.75));
    auto front_sphere = make_shared<lambertian>(color(0, 0, 0));
    auto bottom_sphere = make_shared<lambertian>(color(0.75,0.75,0.75));
    auto top_sphere = make_shared<lambertian>(color(0.75,0.75,0.75));

    auto light_sphere = make_shared<lambertian>(color(12, 12, 12));

    world.add(make_shared<sphere>(point3( 1e5+1,40.8,81.6), 1e5, left_sphere));
    world.add(make_shared<sphere>(point3( -1e5+99,40.8,81.6), 1e5, right_sphere));
    world.add(make_shared<sphere>(point3( 50,40.8, 1e5), 1e5, back_sphere));
    world.add(make_shared<sphere>(point3( 50,40.8,-1e5+170), 1e5, front_sphere));
    world.add(make_shared<sphere>(point3( 50, 1e5, 81.6), 1e5, bottom_sphere));
    world.add(make_shared<sphere>(point3( 50,-1e5+81.6,81.6), 1e5, top_sphere));

    world.add(make_shared<sphere>(point3(50,681.6-.27,81.6), 600, light_sphere));

    auto albedo = color::random(0.5, 1);
    auto fuzz = random_double(0, 0.5);
    auto mirror_sphere = make_shared<metal>(albedo, fuzz);

    auto glass_sphere = make_shared<dielectric>(1.5);

    world.add(make_shared<sphere>(point3(27,16.5,47), 16.5, mirror_sphere));
    world.add(make_shared<sphere>(point3(73,16.5,78), 16.5, glass_sphere));

    return world;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));

    return objects;
}
#endif //PATHTRACINGDEMO_SCENE_H