//
// Created by gabriel on 5/19/22.
//

#ifndef RT_PRIMITIVE_H
#define RT_PRIMITIVE_H

#include "../Vector/ray.h"
#include "Material.h"
#include "Integrator.h"

class Primitive{
protected:
    Material material;
    Integrator* integrator;
public:
    Integrator *getIntegrator() const {
        return integrator;
    }

    void setIntegrator(Integrator *integrator) {
        Primitive::integrator = integrator;
    }

    const Material getMaterial() const {
        return material;
    }

    void setMaterial(const Material &material) {
        Primitive::material = material;
    }

    virtual double intersects(const ray& ray) = 0;

    virtual color getColor(){
        return this->material.getCol();
    }

    virtual void setColor(const color& col_){
        this->material.setCol(col_);
    }
};

#endif //RT_PRIMITIVE_H
