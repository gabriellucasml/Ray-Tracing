//
// Created by gabriel on 07/06/22.
//

#ifndef RT_INTEGRATOR_H
#define RT_INTEGRATOR_H

#include <vector>
#include "../Vector/ray.h"
#include "primitive.h"
#include "../RTUtilities/parcer.h"

class Integrator{
public:
    virtual color ray_color(const ray& r, const struct RunningOptions& ro)  = 0;
};

#endif //RT_INTEGRATOR_H
