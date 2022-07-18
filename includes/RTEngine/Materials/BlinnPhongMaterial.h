#ifndef _BLINNMATERIAL_
#define _BLINNMATERIAL_

#include "Material.h"
#include "../../Vector/vec3.h"
#include "Material.h"
#include "../Objects/Surfel.h"
#include "../../Vector/ray.h"

using Color24 = vec3;

class BlinnMaterial : public Material{

public:
    Color24 color_kd;
    Color24 color_ks;
    Color24 color_ka;
    std::string name;
    vec3 mirror;
    float glossiness;

    ~BlinnMaterial(){};

    BlinnMaterial(vec3 kd, vec3 ks, vec3 ka, std::string name, vec3 mirror, float glossiness);
    void scatter( Surfel *isect, ray *r);
    Color24 kd();
    Color24 ks();
    Color24 ka();
    Color24 km();
    };

    BlinnMaterial::BlinnMaterial(vec3 kd, vec3 ks, vec3 ka, std::string name, vec3 mirror, float glossiness){
        this->color_kd = kd;
        this->color_ks = ks;
        this->color_ka = ka;
        this->name = name;
        this->mirror = mirror;
        this->glossiness = glossiness;
    }

    void BlinnMaterial::scatter( Surfel *isect, ray *r){

    }

    Color24 BlinnMaterial::kd(){
        return color_kd;
    }

    Color24 BlinnMaterial::ks(){
        return color_ks;
    }

    Color24 BlinnMaterial::ka(){
        return color_ka;
    }

    Color24 BlinnMaterial::km(){
        return mirror;
    }

#endif