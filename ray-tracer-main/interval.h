#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

// Constants

const float infinity = 1e20f;
const float pi = 3.1415926f;

class interval {
  public:
    float min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    interval(float min, float max) : min(min), max(max) {}

    inline float size() const {
        return max - min;
    }

    inline bool contains(float x) const {
        return min <= x && x <= max;
    }

    inline bool surrounds(float x) const {
        return min < x && x < max;
    }

    inline float clamp(float x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif