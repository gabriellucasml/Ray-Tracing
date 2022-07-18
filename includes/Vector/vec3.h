#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

public:
    double e[3];

    bool isBlack() {
        return this->e[0]==0 &&  this->e[1]==0 && this->e[2] == 0;
    }
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

inline bool operator==(const vec3 &v1, const vec3 &v2){
    if(v1.e[0]==v2.e[0] && v1.e[1]==v2.e[1] && v1.e[2]==v2.e[2])
        return true;
}

inline bool operator!=(const vec3 &v1, const vec3 &v2){
    if(v1.e[0]==v2.e[0] && v1.e[1]==v2.e[1] && v1.e[2]==v2.e[2])
        return false;
}

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline vec3 operator*(double t, const vec3 &v) {
    return {t*v.e[0], t*v.e[1], t*v.e[2]};
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
           + u.e[1] * v.e[1]
           + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 normalize(const vec3 &v)
{
    float sq_v1 = v.e[0] * v.e[0];
    float sq_v2 = v.e[1] * v.e[1];
    float sq_v3 = v.e[2] * v.e[2];

    float root = sqrt(sq_v1 + sq_v2 + sq_v3);

    float r_v1 = v.e[0] / root;
    float r_v2 = v.e[1] / root;
    float r_v3 = v.e[2] / root;

    return vec3(r_v1, r_v2, r_v3);
}

inline float magnitude(const vec3 &v){
    float sq_v1 = v.e[0] * v.e[0];
    float sq_v2 = v.e[1] * v.e[1];
    float sq_v3 = v.e[2] * v.e[2];

    float root = sqrt(sq_v1 + sq_v2 + sq_v3);
    return root;
}

inline float cosAnguloVetores(const vec3 &v1, const vec3 &v2){
    float cim = v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
    float bai = v1.length() * v2.length();
    return cim/bai;
}

#endif