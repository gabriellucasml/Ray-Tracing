//
// Created by gabriel on 5/19/22.
//

#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "primitive.h"

class Sphere : public Primitive{
private:
    point3 center;
    double radius;
public:
    Sphere(point3 center, double radius, color col){
        this->center = center;
        this->radius = radius;
        this->col = col;
    }

    const point3 &getCenter() const {
        return this->center;
    }

    void setCenter(const point3 &center_) {
        this->center = center_;
    }

    double getRadius() const {
        return this->radius;
    }

    void setRadius(double radius_) {
        this->radius = radius_;
    }

    bool intersects(const ray& r) override{
        vec3 oc = r.origin() - this->center;
        auto a = dot(r.direction(), r.direction());
        auto b = 2.0 * dot(oc, r.direction());
        auto c = dot(oc, oc) - this->radius*this->radius;
        auto discriminant = b*b - 4*a*c;
        return (discriminant > 0);
    }
};

#endif //RT_SPHERE_H
