#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <iostream>

class Vec3{
    private:
    float x;
    float y;
    float z;
    public:
// Constructors
    Vec3(){
        x = 0;
        y = 0;
        z = 0;
    }
    Vec3(float xi,float yi,float zi){
        x = xi;
        y = yi;
        z = zi;
    }
// Accessors
    inline float operator[](int a) const{
        if(a==0) return x;
        if(a==1) return y;
        if(a==2) return z;
        return 0;
    }
    inline float& operator[](int a){
        if(a==0) return x;
        if(a==1) return y;
        if(a==2) return z;
        return x;
    }
    inline Vec3 operator-() const{
        return Vec3(-x,-y,-z);
    }

// Vector operations
    inline float length() const{
        return std::sqrt(x*x + y*y + z*z);
    }
    inline float length_squared() const{
        return (x*x + y*y + z*z);
    }

    inline bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }
};

// operators
inline Vec3 operator+ (const Vec3&a, const Vec3& b){
    return Vec3(a[0]+b[0],a[1]+b[1],a[2]+b[2]);
}
inline Vec3 operator- (const Vec3&a, const Vec3& b){
    return Vec3(a[0]-b[0],a[1]-b[1],a[2]-b[2]);
}
inline Vec3 operator* (float scalar,const Vec3& other){
    return Vec3(other[0] * scalar ,other[1] * scalar ,other[2] * scalar );
}
inline Vec3 operator* (const Vec3& other,float scalar){
    return Vec3(other[0] * scalar ,other[1] * scalar ,other[2] * scalar );
}
inline Vec3 operator/ (const Vec3& other,float scalar){
    return Vec3(other[0] / scalar ,other[1] / scalar ,other[2] / scalar );
}


// Vector Functions
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

float dot(const Vec3& a,const Vec3& b){
    return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
inline Vec3 cross(const Vec3& a,const Vec3& b){
    return Vec3(a[1]*b[2]-a[2]*b[1] , a[2]*b[0]-a[0]*b[2] , a[0]*b[1]-a[1]*b[0]);
}
inline Vec3 unit_vector(const Vec3& a){
    float len = a.length();
    if(len != 0){
        return a / len;
    }
    else{
        return a;
    }
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, float etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

using Point = Vec3;


#endif