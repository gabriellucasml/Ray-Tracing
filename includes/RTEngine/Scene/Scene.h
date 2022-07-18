//
// Created by Gabriel on 18/07/2022.
//

#ifndef RT_SCENE_H
#define RT_SCENE_H

#include <vector>
#include <memory>
#include "primitive.hpp"
#include "Camera.h"
#include "film.hpp"
#include "Background.h"
#include "geometricprimitive.hpp"
#include "light/light.hpp"
#include "light/ambient.hpp"
#include "bounds3.hpp"
#include "bvh.hpp"
#include "bvhaccel.hpp"
#include "../Lights/light.hpp"
#include "../Objects/bounds3.h"
#include "../Lights/ambient.hpp"

class Scene{
public:
    Camera * camera;
    Background * background;
    Film * film;
    std::vector<std::shared_ptr<GeometricPrimitive>> obj_list;
    std::vector<std::shared_ptr<Bounds3>> bounds;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Bvh_node> bvh_node;
    std::shared_ptr<BvhAccel> bvh_;
    AmbientLight * ambient = nullptr;
    Bounds3 worldBound;
    //std::vector<GeometricPrimitive> * obj_list;
    Scene() {}
    Scene(Camera * cam, Background * bg, Film * film, std::vector<std::shared_ptr<GeometricPrimitive>> obj_list);
    ~Scene() = default;
    void setCamera(Camera * camera);
    void setBackground(Background * bg);
    void setFilm(Film * film);
    void setObjList(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list);
    void setBounds(std::vector<std::shared_ptr<Bounds3>> bounds);
    void setLight(std::vector<std::shared_ptr<Light>> lights);
    Bounds3 & getWorldBound() { return worldBound;}
private:


};

Scene::Scene(Camera * cam, Background * bg, Film * film, std::vector<std::shared_ptr<GeometricPrimitive>> obj_list){
    this->camera = cam;
    this->background = bg;
    this->film = film;
    this->obj_list = obj_list;
}

void Scene::setCamera(Camera * camera){
    this->camera = camera;
}

void Scene::setBackground(Background * bg){
    this->background = bg;
}

void Scene::setFilm(Film * film){
    this->film = film;
}

void Scene::setObjList(std::vector<std::shared_ptr<GeometricPrimitive>> obj_list){
    this->obj_list = obj_list;
}

void Scene::setBounds(std::vector<std::shared_ptr<Bounds3>> bounds){
    this->bounds = bounds;
}

void Scene::setLight(std::vector<std::shared_ptr<Light>> lights){
    this->lights = lights;

    Scene s = *this;

    for(auto light : lights){
        light->preprocessLight(s);
    }
}

#endif //RT_SCENE_H
