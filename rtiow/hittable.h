#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

class material;

struct hit_record {
    point3 p; // intersection point
    vec3 normal; // normal vector of point p and ray r
    shared_ptr<material> mat; //hit rec for material pointer
    double t; // distance along ray
    bool front_face; // is normal facing the ray

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // HITTABLE_H
