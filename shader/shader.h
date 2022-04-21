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

    color render(const ray &r, const hittable &world, int depth);

private:
    ray r;
    int depth;
};

color shader::render(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    if (depth <= 0){
        return color(0,0,0);
    }

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if(rec.mat_ptr -> scatter(r, rec, attenuation, scattered)){
            return attenuation * render(scattered, world, depth - 1);
        }
        return color(1, 1, 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

#endif //PATHTRACINGDEMO_SHADER_H
