#ifndef RANDOMH
#define RANDOMH
// have to implement a random number generator here
#include "vec.h"
#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <random>

//Basis

struct xorwow_state_t {
    uint32_t x, y, z, w, v, d;
};

inline uint32_t xorwow(xorwow_state_t& state) {
    uint32_t t = state.x ^ (state.x >> 2);
    state.x = state.y; state.y = state.z;
    state.z = state.w; state.w = state.v;
    state.v = state.v ^ (state.v << 4) ^ (t ^ (t << 1));
    state.d += 362437;
    return state.v + state.d;
}

inline xorwow_state_t make_random_state() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto seed = static_cast<uint64_t>(now.count());

    xorwow_state_t state;
    state.x = static_cast<uint32_t>(seed);
    state.y = static_cast<uint32_t>(seed >> 32);
    state.z = 362436069;
    state.w = 521288629;
    state.v = 88675123;
    state.d = 6615241;
    return state;
}


// Random float in [0,1) IMP

inline float random_float(xorwow_state_t& state) {
    return xorwow(state) * (1.0f / 4294967296.0f); // 2^32
}


// Random float in [min, max)
inline float random_float(float min, float max,xorwow_state_t& state) {
    return min + (max - min) * random_float(state);
}

inline Vec3 randomvec(xorwow_state_t& state){
    return Vec3(random_float(state), random_float(state), random_float(state));
}

inline Vec3 randomvec(float min,float max,xorwow_state_t& state){
    return Vec3(random_float(min,max,state),random_float(min,max,state),random_float(min,max,state));
}

inline Vec3 randomunitvec(xorwow_state_t& state){
    while(true){
        auto p = randomvec(-1.0f,+1.0f,state);
        if(p.length_squared() > 1e-10f && p.length_squared() <= 1.0f){
            return unit_vector(p);
        }
    }
}

inline Vec3 randomonhemi(const Vec3& normal,xorwow_state_t& state) {
    Vec3 on_unit_sphere = randomunitvec(state);
    if (dot(on_unit_sphere, normal) > 0.0f) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}


inline Vec3 random_in_unitdisk(xorwow_state_t& state) {
    while (true) {
        auto p = Vec3(random_float(-1,1,state), random_float(-1,1,state), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

#endif