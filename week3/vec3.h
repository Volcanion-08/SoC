#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
  public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    vec3 unit() const;

    double dot(const vec3& other) const {
        return e[0] * other.e[0]
             + e[1] * other.e[1]
             + e[2] * other.e[2];
    }

    vec3 cross(const vec3& other) const {
        return vec3(e[1] * other.e[2] - e[2] * other.e[1],
                    e[2] * other.e[0] - e[0] * other.e[2],
                    e[0] * other.e[1] - e[1] * other.e[0]);
    }

    vec3 reflect(const vec3& normal);

    vec3 refract(const vec3& normal, double ri);
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

vec3 vec3::unit() const {
    return *this / length();
}

vec3 vec3::reflect(const vec3& normal) {
    vec3 unit_normal = normal.unit();
    return *this - 2*(dot(unit_normal))*unit_normal;
}

vec3 vec3::refract(const vec3& normal, double ri) {
    vec3 unit_normal = normal.unit();
    double sine = cross(unit_normal).length()/(length()*ri);
    if(sine>1) return reflect(normal);
    vec3 unit_planar = (*this - dot(unit_normal)*unit_normal).unit();
    return std::sqrt(1-sine*sine)*unit_normal*((dot(unit_normal)>0)?1:-1) + sine*unit_planar;
}

#endif