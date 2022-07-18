//
// Created by gabriel on 07/06/22.
//

#ifndef RT_DEPTHMAPINTEGRATOR_H
#define RT_DEPTHMAPINTEGRATOR_H

#include "Integrator.h"
#include "../../RTUtilities/parcer.h"

class DepthMapIntegrator : public Integrator{
public:
    color ray_color(const ray& r, const struct RunningOptions& ro)  override{
        for(auto shape : ro.objects){
            auto t = shape->intersects(r);
            if (t > 0.0) {
                vec3 N = r.at(t) - vec3(0, 0, -1);
                auto c = color(std::abs(N.x()), std::abs(N.y()), std::abs(N.z()))*shape->getColor()/255;
                return c;
            }
        }
        vec3 unit_direction;
        if(std::strcmp(ro.cameraType.c_str(), "orthographic")==0)
            unit_direction = unit_vector(r.origin());
        else
            unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        color c1 = (1.0-t)*ro.backgroundBl/255 + t*ro.backgroundTr/255;
        color c2 = (1.0-t)*ro.backgroundBr/255 + t*ro.backgroundTl/255;
        auto h = 0.5*(unit_direction.x() + 1.0);
        return (1.0-h)*c1 + h*c2;
    }
};

#endif //RT_DEPTHMAPINTEGRATOR_H
