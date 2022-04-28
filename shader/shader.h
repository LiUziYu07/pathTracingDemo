//
// Created by lzy on 2022/4/18.
//

#ifndef PATHTRACINGDEMO_SHADER_H
#define PATHTRACINGDEMO_SHADER_H

#include "../tool/color.h"
#include "../hit/hittable.h"
#include "../tool/pdf.h"

class shader{
public:
    shader() {}
    shader(const ray& r, const int depth){}

    color pathTracing(const ray &r, const hittable &world, int depth, color background, shared_ptr<hittable>& lights);

    color whittedStyleRT(const ray &r, const hittable &world, int depth, color background, shared_ptr<hittable> &lights);

private:
    ray r;
    int depth;
};

color shader::pathTracing(const ray &r, const hittable &world, int depth, color background, shared_ptr<hittable>& lights) {
    hit_record rec;

    double p_rr = random_double();
    if (depth > 20){
        if(p_rr < random_double()){
            return color(0, 0, 0);
        }
    }
    else if(depth >= 50){
        return color(0, 0, 0);
    }

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color emitted = rec.mat_ptr -> emitted(r, rec, rec.u, rec.v, rec.p);

    color albedo;
    double pdf_val;
    bool is_specular;

    if (!rec.mat_ptr -> scatter(r, rec, albedo, scattered, pdf_val, is_specular)) {
        return emitted;
    }

    if(is_specular){
        return albedo
        * pathTracing(scattered, world, depth + 1, background, lights);
    }

    auto p0 = make_shared<hittable_pdf>(lights, rec.p);
    auto p1 = make_shared<cosine_pdf>(rec.normal);
    mixture_pdf mixed_pdf(p0, p1);

    scattered = ray(rec.p, mixed_pdf.generate(), r.time());
    pdf_val = mixed_pdf.value(scattered.direction());


    return emitted + albedo * rec.mat_ptr -> scattering_pdf(r, rec, scattered)
    * pathTracing(scattered, world, depth + 1, background, lights) / pdf_val;
}

color shader::whittedStyleRT(const ray &r, const hittable &world, int depth, color background, shared_ptr<hittable>& lights) {
    hit_record rec;

    if (depth <= 0){
        return color(0,0,0);
    }

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color emitted = rec.mat_ptr -> emitted(r, rec, rec.u, rec.v, rec.p);

    color albedo;
    double pdf_val;
    bool is_specular;

    if (!rec.mat_ptr -> scatter(r, rec, albedo, scattered, pdf_val, is_specular)) {
        return emitted;
    }

    if(is_specular){
        return albedo
               * pathTracing(scattered, world, depth - 1, background, lights);
    }
    return background + emitted + albedo;
}
#endif //PATHTRACINGDEMO_SHADER_H
