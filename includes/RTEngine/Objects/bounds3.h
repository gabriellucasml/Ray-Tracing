#ifndef _BOUNDS3_
#define _BOUNDS3_

#include <limits>
#include <algorithm>
#include "../../Vector/vec3.h"
#include "../../Vector/ray.h"

using Point = vec3;
#include <memory>

class GeometricPrimitive;

class Bounds3
{
public:
    Point pMin;
    Point pMax;

    std::shared_ptr<GeometricPrimitive> geo;
    // std::shared_ptr<GeometricPrimitive> left;
    // std::shared_ptr<GeometricPrimitive> right;

    Bounds3()
    {
        float minNum = std::numeric_limits<float>::lowest();
        float maxNum = std::numeric_limits<float>::max();
        pMin = Point(maxNum, maxNum, maxNum);
        pMax = Point(minNum, minNum, minNum);
        //geo = std::make_shared<GeometricPrimitive>();
    }

    Bounds3(std::shared_ptr<Bounds3> b){
        pMin = b.get()->pMin;
        pMax = b.get()->pMax;
        geo = b.get()->geo;
        // left = b.get()->left;
        // right = b.get()->right;
    }

    virtual ~Bounds3() = default;

    // void setLeftGeoPrimitive(std::shared_ptr<GeometricPrimitive> geo){
    //     this->left = geo;
    // }

    // void setRightGeoPrimitive(std::shared_ptr<GeometricPrimitive> geo){
    //     this->right = geo;
    // }

    void setGeoPrimitive(std::shared_ptr<GeometricPrimitive> geo){
        this->geo = geo;
    }

    Bounds3(const Point &p) : pMin(p), pMax(p) {}

    Bounds3(const Point &p1, const Point &p2) : pMin(std::min(p1.e[0], p2.e[0]), std::min(p1.e[1], p2.e[1]),
                                                     std::min(p1.e[2], p2.e[2])),
                                                pMax(std::max(p1.e[0], p2.e[0]), std::max(p1.e[1], p2.e[1]),
                                                     std::max(p1.e[2], p2.e[2])) {}

    const Point &operator[](int i) const;
    Point &operator[](int i);

    bool operator==(const Bounds3 &b) const
    {
        return b.pMin == pMin && b.pMax == pMax;
    }

    bool operator!=(const Bounds3 &b) const
    {
        return b.pMin != pMin || b.pMax != pMax;
    }

    Point corner(int corner) const
    {
        return Point((*this)[(corner & 1)].e[0],
                     (*this)[(corner & 2) ? 1 : 0].e[1],
                     (*this)[(corner & 4) ? 1 : 0].e[2]);
    }

    vec3 diagonal()
    {
        vec3 p = pMax - pMin;
        return p;
    }

    float surfaceArea()
    {
        vec3 d = diagonal();
        return 2 * (d.e[0] * d.e[1] + d.e[0] * d.e[2] + d.e[1] * d.e[2]);
    }

    float volume()
    {
        vec3 d = diagonal();
        return d.e[0] * d.e[1] * d.e[2];
    }

    int maximumExtent()
    {
        vec3 d = diagonal();
        if (d.e[0] > d.e[1] && d.e[0] > d.e[2])
            return 0;
        else if (d.e[1] > d.e[2])
            return 1;
        else
            return 2;
    }

    vec3 offset(Point &p)
    {
        vec3 o = p - pMin;
        if (pMax.e[0] > pMin.e[0])
            o.e[0] /= pMax.e[0] - pMin.e[0];
        if (pMax.e[1] > pMin.e[1])
            o.e[1] /= pMax.e[1] - pMin.e[1];
        if (pMax.e[2] > pMin.e[2])
            o.e[2] /= pMax.e[2] - pMin.e[2];
        return o;
    }

    void boundingSphere(Point *center, float *radius)
    {
        *center = (pMin + pMax) / 2;
        *radius = inside(*center, *this) ? distance(*center, pMax) : 0;
    }

    bool intersect_p(ray &ray, float *hitt0 = nullptr, float *hitt1 = nullptr)
    {
        float t0 = ray.tmin;
        float t1 = ray.tmax;

        float invRayDir_v1 = 1.f / ray.direction().e[0];
        float tNear_v1 = invRayDir_v1 * (pMin.e[0] - ray.origin().e[0]);
        float tFar_v1 = (pMax.e[0] - ray.origin().e[0]) * invRayDir_v1;

        if(tNear_v1 > tFar_v1) std::swap(tNear_v1, tFar_v1);
        t0 = tNear_v1 > t0 ? tNear_v1 : t0;
        t1 = tFar_v1 < t1 ? tFar_v1 : t1;
        if(t0 > t1 and t0 > 0 and t1 > 0) return false;

        float invRayDir_v2 = 1.f / ray.direction().e[1];
        float tNear_v2 = invRayDir_v2 * (pMin.e[1] - ray.origin().e[1]);
        float tFar_v2 = (pMax.e[1] - ray.origin().e[1]) * invRayDir_v2;

        if(tNear_v2 > tFar_v2) std::swap(tNear_v2, tFar_v2);
        t0 = tNear_v2 > t0 ? tNear_v2 : t0;
        t1 = tFar_v2 < t1 ? tFar_v2 : t1;
        if(t0 > t1 and t0 > 0 and t1 > 0) return false;

        float invRayDir_v3 = 1.f / ray.direction().e[2];
        float tNear_v3 = invRayDir_v3 * (pMin.e[2] - ray.origin().e[2]);
        float tFar_v3 = (pMax.e[2] - ray.origin().e[2]) * invRayDir_v3;

        if(tNear_v3 > tFar_v3) std::swap(tNear_v3, tFar_v3);
        t0 = tNear_v3 > t0 ? tNear_v3 : t0;
        t1 = tFar_v3 < t1 ? tFar_v3 : t1;
        if(t0 > t1 and t0 > 0 and t1 > 0) return false;

        if (hitt0)
            *hitt0 = t0;
        if (hitt1)
            *hitt1 = t1;



        return true;
    }
    inline bool interset_p(const ray &ray, const vec3 &invDir, const int dirIsneg[3]) const;

    bool inside(const Point &p, const Bounds3 &b)
    {
        return (p.e[0] >= b.pMin.e[0] && p.e[0] <= b.pMax.e[0] &&
                p.e[1] >= b.pMin.e[1] && p.e[1] <= b.pMax.e[1] &&
                p.e[2] >= b.pMin.e[2] && p.e[2] <= b.pMax.e[2]);
    }

    float distance(Point &p1, Point &p2)
    {
        return (p1 - p2).length();
    }

    Point lerp(Point &t)
    {
        return Point(lerp_f(t.e[0], pMin.e[0], pMax.e[0]),
                     lerp_f(t.e[1], pMin.e[1], pMax.e[1]),
                     lerp_f(t.e[2], pMin.e[2], pMax.e[2]));
    }

    float lerp_f(float t, float v1, float v2)
    {
        return (1 - t) * v1 + t * v2;
    }
};

//Realiza a união de 2 bounds
std::shared_ptr<Bounds3> unionBounds(const Bounds3 &b1, const Bounds3 &b2)
{
    std::shared_ptr<Bounds3> b = std::make_shared<Bounds3>(Point(std::min(b1.pMin.e[0], b2.pMin.e[0]),
                                                                 std::min(b1.pMin.e[1], b2.pMin.e[1]),
                                                                 std::min(b1.pMin.e[2], b2.pMin.e[2])),
                                                                 Point(std::max(b1.pMax.e[0], b2.pMax.e[0]),
                                                                 std::max(b1.pMax.e[1], b2.pMax.e[1]),
                                                                 std::max(b1.pMax.e[2], b2.pMax.e[2])));
    // b->left = b1.left;
    // b->right = b2.right;
    return b;

}

Bounds3 unionBounds(const Bounds3 &b, const Point &p)
{
    return Bounds3(Point(std::min(b.pMin.e[0], p.e[0]),
                         std::min(b.pMin.e[1], p.e[1]),
                         std::min(b.pMin.e[2], p.e[2])),
                         point3(std::max(b.pMax.e[0], p.e[0]),
                         std::max(b.pMax.e[1], p.e[1]),
                         std::max(b.pMax.e[2], p.e[2])));
}

#endif
