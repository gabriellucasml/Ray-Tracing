//
// Created by Gabriel on 18/07/2022.
//

#ifndef RT_BACKGROUND_H
#define RT_BACKGROUND_H
#include <string>
#include "../../Vector/vec3.h"

class Background
{
private:
    /* data */

    std::string type;
    vec3 bl;
    vec3 br;
    vec3 tl;
    vec3 tr;
    vec3 color;
    std::string mapping;

public:
    Background(std::string type, vec3 color);
    Background(std::string type, vec3 bl, vec3 br, vec3 tl, vec3 tr, std::string mapping);
    ~Background();
    std::string getType();
    std::string getMapping();
    vec3 sample(float v1, float v2);
    vec3 interpolate(float x, float y);
};

Background::Background(std::string type, vec3 color)
{
    this->type = type;
    this->color = color;
}

Background::Background(std::string type, vec3 bl, vec3 br, vec3 tl, vec3 tr, std::string mapping){
    this->type = type;
    this->bl = bl;
    this->br = br;
    this->tl = tl;
    this->tr = tr;
    this->mapping = mapping;
}

Background::~Background() = default;


std::string Background::getMapping(){
    return mapping;
}

std::string Background::getType(){
    return type;
}

vec3 Background::sample(float v1, float v2){
    if(!color.isBlack()){
        return color;
    } else{
        vec3 interpolated = interpolate(v1, v2);
        return interpolated;
    }
}

vec3 Background::interpolate(float x, float y){
    float v1_ = bl.e[0]*(1-x)*(1-y) + br.e[0]*x*(1-y) + tl.e[0]*(1-x)*y + tr.e[0]*x*y;
    float v2_ = bl.e[1]*(1-x)*(1-y) + br.e[1]*x*(1-y) + tl.e[1]*(1-x)*y + tr.e[1]*x*y;
    float v3_ = bl.e[2]*(1-x)*(1-y) + br.e[2]*x*(1-y) + tl.e[2]*(1-x)*y + tr.e[2]*x*y;

    return {v1_, v2_, v3_};
}
#endif //RT_BACKGROUND_H
