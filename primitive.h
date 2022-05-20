//
// Created by gabriel on 5/19/22.
//

#ifndef RT_PRIMITIVE_H
#define RT_PRIMITIVE_H

#include "ray.h"

class Primitive{
protected:
    color col;
public:
    virtual bool intersects(const ray& ray) = 0;

    color getColor(){
        return this->col;
    }
    void setColor(const color& col_){
        this->col = col_;
    }
};

#endif //RT_PRIMITIVE_H
