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
    Sphere(point3 center, double radius, Integrator* integrator){
        this->center = center;
        this->radius = radius;
        this->integrator = integrator;
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

    double intersects(const ray& r) override{
        vec3 oc = r.origin() - this->center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;
        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) {
            return -1.0;
        } else {
            return (-half_b - sqrt(discriminant) ) / a;
        }
    }
};

#endif //RT_SPHERE_H
