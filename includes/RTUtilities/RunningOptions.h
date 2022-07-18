//
// Created by gabriel on 07/06/22.
//

#ifndef RT_RUNNINGOPTIONS_H
#define RT_RUNNINGOPTIONS_H

#include <string>
#include "../Vector/color.h"
#include "../RTEngine/Objects/primitive.h"
#include "../RTEngine/Lights/light.hpp"
#include "../RTEngine/Lights/ambient.hpp"

const struct RunningOptions{
    bool  help = false;
    std::string cameraType;
    int fovy = 1;
    std::vector<int> screen_window{1,1,1,1};
    std::vector<Primitive*> objects;
    std::vector<std::shared_ptr<Light>>lights;
    AmbientLight * ambient = nullptr;
    std::string filmType;
    int filmX_res = 0;
    int filmY_res = 0;
    std::string filmFilename;
    std::string filmImgtype;
    std::string backgroundType = "color";
    std::string backgroundMapping = "screen";
    color backgroundColor = color(0,0,0);
    color backgroundBl = color(0,0,0);
    color backgroundBr = color(0,0,0);
    color backgroundTl = color(0,0,0);
    color backgroundTr = color(0,0,0);
    std::string integratorType;
} RunningOptions;

#endif //RT_RUNNINGOPTIONS_H
