//
// Created by lzy on 2022/4/18.
//

#ifndef PATHTRACINGDEMO_SHADER_H
#define PATHTRACINGDEMO_SHADER_H

#include "../tool/color.h"
#include "../hit/hittable.h"

class shader{
public:
    shader() {}
    shader(const ray& r, const int depth){}

    color render(const ray &r, const hittable &world, int depth, color background);

private:
    ray r;
    int depth;
};

color shader::render(const ray &r, const hittable &world, int depth, color background) {
    hit_record rec;

    if (depth <= 0){
        return color(0,0,0);
    }

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color emitted = rec.mat_ptr -> emitted(r, rec, rec.u, rec.v, rec.p);

    color albedo;
    double pdf;

    if (!rec.mat_ptr -> scatter(r, rec, albedo, scattered, pdf))
        return emitted;

    auto on_light = point3(random_double(213, 343), 554, random_double(227, 332));
    auto to_light = on_light - rec.p;
    auto distance_squared = to_light.length_squared();
    to_light = unit_vector(to_light);

    if(dot(to_light, rec.normal) < 0){
        return emitted;
    }

    double light_area = (343 - 213) * (332 - 227);
    auto light_cosine = fabs((to_light.y()));
    if(light_cosine < 0.000001){
        return emitted;
    }
    pdf = distance_squared / (light_cosine * light_area);
    scattered = ray(rec.p, to_light, r.time());

    return emitted + albedo * rec.mat_ptr -> scattering_pdf(r, rec, scattered)
    * render(scattered, world, depth-1, background) / pdf;
}

#endif //PATHTRACINGDEMO_SHADER_H
