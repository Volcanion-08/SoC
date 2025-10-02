#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "material.h"

class hit_record {
  public:
    Point p;
    Vec3 normal;
    float t;
    bool front_face;
    Material mat;

    inline void setnormal(const Ray& r, const Vec3& outward_normal){

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};


// Getting it ready for the future use if required
enum class ObjectType{
    Sphere,
};

#endif