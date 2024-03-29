//
// Created by lzy on 2022/4/14.
//

#ifndef PATHTRACINGDEMO_MATERIAL_H
#define PATHTRACINGDEMO_MATERIAL_H

#include "../constant/constant.h"
#include "../hit/hittable.h"
#include "texture.h"
#include "../tool/onb.h"
#include "../tool/pdf.h"

struct hit_record;
class material{
public:
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf, bool& is_specular
    ) const {
        return false;
    }

    virtual color emitted(const ray& r_in, const hit_record& rec ,double u, double v,
                          const point3& p) const {
        return color(0,0,0);
    }

    virtual double scattering_pdf(
            const ray& r_in, const hit_record& rec, ray& scattered
            ) const {
        auto cosine = dot(unit_vector(scattered.direction()), rec.normal);
        return cosine < 0 ? 0 : cosine / pi;
    }
};

class lambertian : public material{
public:
    lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& alb, ray& scattered, double& pdf, bool& is_specular
    ) const override{
        /*
        auto scatter_direction = random_in_hemisphere(rec.normal);

        scattered = ray(rec.p, unit_vector(scatter_direction), r_in.time());
        alb = albedo -> value(rec.u, rec.v, rec.p);
        pdf = 0.5 / pi;
        return true;*/
        is_specular = false;
        onb uvw;
        uvw.build_from_w(rec.normal);
        auto scatter_direction = uvw.local(random_cosine_direction());
        scattered = ray(rec.p, unit_vector(scatter_direction), r_in.time());
        alb = albedo -> value(rec.u, rec.v, rec.p);
        pdf = dot(uvw.w(), scattered.direction()) / pi;
        return true;
    }

    double scattering_pdf(
            const ray& r_in, const hit_record& rec, ray& scattered
            ) const {
        auto cosine = dot(unit_vector(scattered.direction()), rec.normal);
        return cosine < 0 ? 0 : cosine / pi;
    }

public:
    shared_ptr<texture> albedo;
};

class metal : public material{
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1){}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf, bool& is_specular
    ) const override{
        is_specular = true;
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

class dielectric : public  material{
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf, bool& is_specular
    ) const override{
        is_specular = true;
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){
            direction = reflect(unit_direction, rec.normal);
        }
        else{
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }
public:
    double ir;

private:
    static double reflectance(double cosine, double ref_idx){
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

class diffuse_light : public material  {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf, bool& is_specular
    ) const override {
        return false;
    }

    virtual color emitted(const ray& r_in, const hit_record& rec ,double u, double v,
                          const point3& p) const override {
        if(rec.front_face){
            return emit -> value(u, v, p);
        }
        else{
            return color(0, 0, 0);
        }
    }

public:
    shared_ptr<texture> emit;
};

#endif //PATHTRACINGDEMO_MATERIAL_H