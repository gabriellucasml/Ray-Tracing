//
// Created by gabriel on 07/06/22.
//

#ifndef RT_NORMALMAPINTEGRATOR_H
#define RT_NORMALMAPINTEGRATOR_H

#include "Integrator.h"
#include "../RTUtilities/parcer.h"

class NormalMapIntegrator : public Integrator{
public:
    color ray_color(const ray& r, const struct RunningOptions& ro) override {

    }
};

#endif //RT_NORMALMAPINTEGRATOR_H
