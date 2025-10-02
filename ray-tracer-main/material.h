#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitlist.h"
#include "random.h"

enum MaterialType {
    LAMBERTIAN,
    METAL,
    DIELECTRIC
};

class hit_record;

struct Material{
    MaterialType type;
    Color albedo;
    float fuzz;       // Only used for metal
    float ref_idx;

    Material() : type(LAMBERTIAN), albedo(Color(0, 0, 0)), fuzz(0.0f), ref_idx(1.0f) {}

    Material(int numtype,Color albedo = Color(0,0,0),float fuzz = 0.0f,float ref_idx = 1.0f): albedo(albedo), fuzz(fuzz), ref_idx(ref_idx){
        if(numtype==1){
            type = MaterialType::LAMBERTIAN;
        }
        if(numtype==2){
            type = MaterialType::METAL;
        }
        if(numtype==3){
            type = MaterialType::DIELECTRIC;
        }
    }
};

Material Lambertian(Color albedo){
    return Material(1,albedo,0.0f,1.0f);
}

Material Metal(Color albedo,float fuzz){
    return Material(2,albedo,fuzz,1.0f);
}

Material Dielectric(float ref_idx = 1.0f){
    return Material(3,Color(0,0,0),0.0f,ref_idx);
}

float reflectance(float cosine, float refraction_index) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return r0 + (1-r0)*std::pow((1 - cosine),5);
}

#endif