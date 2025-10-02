#ifndef SPHERE_H
#define SPHERE_H

#include "vec.h"
#include "ray.h"
#include "interval.h"
#include "hitlist.h"
#include "material.h"

// chaneg std to sycl for sqrt and fmax

struct Sphere{
    // Members
    Point center;
    float radius;
    Material mat;

    
    Sphere() = default;

    Sphere(const Point& centerpoint, float radiusval,Material& material){
        center = centerpoint; 
        radius = std::fmax(0.0f,radiusval); // here
        mat = material;
    }

    inline bool hit(const Ray& r, interval range, hit_record& rec) const {
        Vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant); // here

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!range.contains(root)) {
            root = (h + sqrtd) / a;
            if (!range.contains(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at_t(rec.t);
        Vec3 out_normal = (rec.p - center) / radius;
        rec.setnormal(r,out_normal);
        rec.mat = mat;

        return true;
    }
};



#endif