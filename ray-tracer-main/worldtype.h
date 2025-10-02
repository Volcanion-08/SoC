#ifndef WORLDTYPE_H
#define WORLDTYPE_H

#include "sphere.h"
#include "hitlist.h"

struct WorldObject {
    ObjectType type;
    Sphere sphere;
    WorldObject(const Sphere& s): type(ObjectType::Sphere), sphere(s) {}

    inline bool hit(const Ray& r, interval range, hit_record& rec) const {
        switch (type) {
            case ObjectType::Sphere: return sphere.hit(r, range, rec);
            default: return false;
        }
    }
};

#endif