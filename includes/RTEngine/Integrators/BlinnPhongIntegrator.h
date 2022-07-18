//
// Created by Gabriel on 18/07/2022.
//

#ifndef RT_BLINNPHONGINTEGRATOR_H
#define RT_BLINNPHONGINTEGRATOR_H


#include <memory>
#include "Integrator.h"
#include "../Objects/Surfel.h"
#include "../Scene/Scene.h"
#include "../Materials/BlinnPhongMaterial.h"

using Color24 = vec3;

class BlinnPhongIntegrator : public Integrator{
public:
    color ray_color(const ray &r, const struct RunningOptions &ro) override;
};

color BlinnPhongIntegrator::ray_color(const ray& r, const struct RunningOptions& ro) {

    color color_ = ro.backgroundColor;
    auto lights = ro.lights;

    Surfel sf;

    std::shared_ptr<Surfel> surfel = std::make_shared<Surfel>();
    for (auto shape: ro.objects) {
        if (!(shape->intersects(r))){
            return color_;
        }
        else
        {
            vec3 c;
            vec3 wi;
            //BlinnMaterial *bm = dynamic_cast<BlinnMaterial *>(surfel->primitive->get_material());
            std::string name = "teste";
            BlinnMaterial *bm = new BlinnMaterial(c,c,c,name,c,2.0);
            vec3 n = normalize(surfel->n);
            for (int i = 0; i < lights.size(); i++)
            {
                vec3 l = lights[i]->sample_Li(*surfel.get(), r.origin(), &wi);
                ray shadow_ray;
                if (lights[i]->type == "directional")
                {
                    shadow_ray = ray(surfel->p, l);
                }
                else
                {
                    float dis = distance(surfel->p, lights[i]->from);
                    shadow_ray = ray(surfel->p, l, 0.0, dis);
                    vec3 d_ = surfel->p - lights[i]->from;
                }

                bool hittou = false;

                if (!hittou)
                {
                    float cosTheta = cosAnguloVetores(surfel->n, l);

                    vec3 v = r.origin() - surfel->p;
                    v = normalize(v);

                    vec3 h = (v + l) / (magnitude(v + l)); // * magnitude(dir_)));

                    c = c + (bm->kd() * wi * std::max(0.0, dot(n, l))) + (bm->ks() * wi * std::pow(std::max(0.0, dot(n, h)), bm->glossiness));
                }

                if (ro.ambient != nullptr){
                    vec3 la = ro.ambient->l;
                    color_ = c + (bm->ka() * la);
                } else {
                    color_ = c;
                }

                if (color_.e[0] > 1.0)
                {
                    color_.e[0] = 1.0;
                }

                if (color_.e[1] > 1.0)
                {
                    color_.e[1] = 1.0;
                }

                if (color_.e[2] > 1.0)
                {
                    color_.e[2] = 1.0;
                }
            }
        }
    }
    return color_;
};
#endif //RT_BLINNPHONGINTEGRATOR_H
