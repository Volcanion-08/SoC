#ifndef RAYH
#define RAYH

#include "vec.h"

class Ray{
    private:
        Point orig;
        Vec3 dir;
    public:
        Ray() {}
        
        Ray(const Point& origin, const Vec3& direction) : orig(origin), dir(direction) {}

        inline Point at_t(float t) const {
            return orig + dir * t;
        }
        inline Point origin() const{
            return orig;
        }
        inline Vec3 direction() const{
            return dir;
        }
};

#endif