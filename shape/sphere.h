//
// Created by lzy on 2022/4/14.
//

#ifndef PATHTRACINGDEMO_SPHERE_H
#define PATHTRACINGDEMO_SPHERE_H

#include "../hit/hittable.h"
#include "../tool/vec3.h"

class sphere : public hittable{
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {}


    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    bool hasEmission();

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
    color emission = color(0, 0, 0);
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;
    if(discriminant < 0){
        return false;
    }
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root){
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root){
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    vec3 out_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, out_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::hasEmission(){
    return sphere::emission.length_squared() > 0;
}

#endif //PATHTRACINGDEMO_SPHERE_H
