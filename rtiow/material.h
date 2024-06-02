#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "hittable.h"


class hit_record;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo; //whiteness
};

class metal : public material {
public:
    //metal(const color& albedo) : albedo(albedo) {} // for shiny metal
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {} //for fuzzy metal

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector()); // fuzzy metal
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            //return true;
            return (dot(scattered.direction(), rec.normal) > 0); // fuzzy metal
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material { //mainly for glass and mirror
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
        const override {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
       /* vec3 refracted = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, refracted); */ // for always refract

        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) // only put "cannot_refract" for air bubble
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction); // change this refracted for always refraction
        return true;
    }

private:
    //index of air, vacuum or ratio of materials refractive index / refractive index of enclose material
    double refraction_index;

    static double reflectance(double cosine, double refraction_index) { //schlick's approx for reflect
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif