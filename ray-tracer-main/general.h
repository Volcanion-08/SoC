#ifndef GENERALH
#define GENERALH

#include "vec.h"
#include "ray.h"
#include "color.h"
#include "random.h"
#include <vector>
#include <limits>
#include <iostream>
#include <cmath>
#include <cstdlib>

// helpful functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}
#endif