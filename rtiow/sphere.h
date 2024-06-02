#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, double radius, std::shared_ptr<material> m)
        : center(center), radius(fmax(0, radius)), mat_ptr(m) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < ray_tmin || root > ray_tmax) {
            root = (-half_b + sqrtd) / a;
            if (root < ray_tmin || root > ray_tmax)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat_ptr;

        return true;
    }

private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

#endif // SPHERE_H
