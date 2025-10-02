#ifndef SCATTERH
#define SCATTERH

#include "general.h"
#include "material.h"
#include "hitlist.h"

inline bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered, xorwow_state_t& rng) {
    if(rec.mat.type == MaterialType::LAMBERTIAN){
        auto scatter_direction = rec.normal + randomunitvec(rng);
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scatter_direction = unit_vector(scatter_direction);
        scattered = Ray(rec.p, scatter_direction);
        attenuation = rec.mat.albedo;
        return true;
    }
    if(rec.mat.type == MaterialType::METAL){
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(unit_vector(reflected) + (rec.mat.fuzz * randomunitvec(rng)));
        scattered = Ray(rec.p, reflected);
        attenuation = rec.mat.albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    if(rec.mat.type == MaterialType::DIELECTRIC){
        attenuation = Color(1.0, 1.0, 1.0);
        float ri = rec.front_face ? (1.0/rec.mat.ref_idx) : rec.mat.ref_idx;

        Vec3 unit_direction = unit_vector(r_in.direction());
        float cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        float sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        Vec3 direction;
        if(cannot_refract || reflectance(cos_theta, ri) > random_float(rng)){
            direction = reflect(unit_direction, rec.normal);
        }
        else{
            direction = refract(unit_direction, rec.normal, ri);
        }
        scattered = Ray(rec.p, direction);
        return true;
    }
    return false;
}

#endif