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
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    double pdf;
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf))
        return emitted;

    return emitted + attenuation * render(scattered, world, depth-1, background);
}

#endif //PATHTRACINGDEMO_SHADER_H
