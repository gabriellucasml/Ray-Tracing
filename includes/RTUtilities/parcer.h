//
// Created by gabriel on 4/28/22.
//

#ifndef RT_PARCER_H
#define RT_PARCER_H

#include <string>
#include <sstream>
#include <vector>
#include "tinyxml2.h"
#include "../RTEngine/Objects/primitive.h"
#include "../RTEngine/Objects/sphere.h"
#include "../RTEngine/Integrators/Integrator.h"
#include "../RTEngine/Integrators/FlatIntegrator.h"
#include "../RTEngine/Integrators/DepthMapIntegrator.h"
#include "../RTEngine/Materials/NormalMapIntegrator.h"
#include "../RTUtilities/RunningOptions.h"


class Parser{
public:
    static struct RunningOptions parse(int argc, char* argv[]){
        if(argc < 2 or argc > 8){
            throw std::invalid_argument("No arguments provided");
        }else{
            struct RunningOptions ro;
            switch(argc){
                case 2:
                    if(strcmp(argv[1],"--help") == 0){
                        std::cout << "Usage: RT [<options>] \"input_scene_file\"\nRendering simulation options:\n\t--help                     Print this help text.\n\t--cropwindow x0,x1,y0,y1   Specify an image crop window.\n\t--quick                    Reduces quality parameters to render image quickly.\n\t--outfile <filename>       Write the rendered image to <filename>." << std::endl;
                        ro.help = true;
                        return ro;
                    }else if(strcmp(argv[1],"--quick") == 0 or strcmp(argv[1],"--cropWindow") == 0 or strcmp(argv[1], "--outfile") == 0){
                        throw std::invalid_argument("Scene file path not informed");
                    }else {
                        tinyxml2::XMLDocument doc;
                        if (doc.LoadFile(argv[1]) == 0) {
                            auto RT3 = doc.RootElement();

                            ro.cameraType = RT3->FirstChildElement("camera")->Attribute("type");
                            if(std::strcmp(ro.cameraType.c_str(), "perspective")==0){
                                try {
                                    ro.fovy = std::stoi(RT3->FirstChildElement("camera")->Attribute("fovy"));
                                }catch(std::exception& e){
                                    std::cout << "Unable to determine fovy. Setting to default" << std::endl;
                                }
                            }else if(std::strcmp(ro.cameraType.c_str(), "orthographic") == 0){
                                std::string sScreen_window = RT3->FirstChildElement("camera")->Attribute("screen_window");
                                const char *token = std::strtok(const_cast<char *>(sScreen_window.c_str()), " ");
                                int i = 0;
                                while(token != nullptr){
                                    try{
                                        ro.screen_window[i] = std::stoi(token);
                                        token = std::strtok(nullptr, " ");
                                        i++;
                                    }catch(std::exception& e){
                                        std::cout<<"Unable to retrieve screen window coordinates. Setting to default." << std::endl;
                                    }
                                }
                            }
                            ro.filmType = RT3->FirstChildElement("film")->Attribute("type");
                            try {
                                ro.filmX_res = std::stoi(RT3->FirstChildElement("film")->Attribute("x_res"));
                            } catch(std::exception& e) {
                                std::cout<<"Unable to read x_res field in field tag from Scene File, setting to default value" << std::endl;
                                ro.filmX_res = 800;
                            }
                            try {
                                ro.filmY_res = std::stoi(RT3->FirstChildElement("film")->Attribute("y_res"));
                            } catch(std::exception& e) {
                                std::cout<<"Unable to read y_res field in field tag from Scene File, setting to default value" << std::endl;
                                ro.filmY_res = 600;
                            }
                            ro.filmFilename = RT3->FirstChildElement("film")->Attribute("filename");
                            ro.filmImgtype = RT3->FirstChildElement("film")->Attribute("img_type");

                            auto background = RT3->FirstChildElement("background");
                            ro.backgroundType = background->Attribute("type");
                                if(background->Attribute("mapping"))
                                    ro.backgroundMapping = background->Attribute("mapping");
                                else
                                    std::cout << "Unable to retrieve background mapping. Setting to default." << std::endl;
                            if (background->Attribute("color")) {
                            std::string aux = background->Attribute("color");
                            color c(0,0,0);
                            char *token = std::strtok(const_cast<char *>(aux.c_str()), " ");
                            int i = 0;
                            while(token != nullptr){
                                try {
                                    c[i] = std::stoi(token);
                                    token = std::strtok(nullptr, " ");
                                    i++;
                                }catch(std::exception& e){
                                    std::cout << "Failed to retrieve color vector. Setting to default value" << std::endl;
                                }
                            }
                            ro.backgroundColor = c;
                            ro.backgroundBl = c;
                            ro.backgroundBr = c;
                            ro.backgroundTl = c;
                            ro.backgroundTr = c;
                            } else {
                                std::string auxbl = background->Attribute("bl"), auxbr = background->Attribute("br"), auxtl = background->Attribute("tl"), auxtr = background->Attribute("tr");
                                color bl(0,0,0),tl(0,0,0),br(0,0,0),tr(0,0,0);
                                std::vector<std::string> tokensBl, tokensBr, tokensTl, tokensTr;
                                std::stringstream sstreamBl(auxbl), sstreamBr(auxbr), sstreamTl(auxtl), sstreamTr(auxtr);
                                std::string interBl, interBr, interTl, interTr;
                                while(std::getline(sstreamBl, interBl, ' '))
                                    tokensBl.push_back(interBl);
                                while(std::getline(sstreamBr, interBr, ' '))
                                    tokensBr.push_back(interBr);
                                while(std::getline(sstreamTl, interTl, ' '))
                                    tokensTl.push_back(interTl);
                                while(std::getline(sstreamTr, interTr, ' '))
                                    tokensTr.push_back(interTr);
                                for(int i = 0; i < 3; i++){
                                    try{
                                        bl[i] = std::stoi(tokensBl[i]);
                                        br[i] = std::stoi(tokensBr[i]);
                                        tl[i] = std::stoi(tokensTl[i]);
                                        tr[i] = std::stoi(tokensTr[i]);
                                    }catch(std::exception& e){
                                        std::cout<< "Unable to process colors, setting default color" << std::endl;
                                        return ro;
                                    }
                                }
                                ro.backgroundBl = bl;
                                ro.backgroundBr = br;
                                ro.backgroundTl = tl;
                                ro.backgroundTr = tr;
                            }

                            //Material
                            if(RT3->FirstChildElement("material")){
                                auto materialElement = RT3->FirstChildElement("material");
                                do{
                                    auto material = new Material();
                                    Integrator* integrator;
                                    if(materialElement->Attribute("type")) {
                                        material->setType(materialElement->Attribute("type"));
                                        if (std::strcmp(material->getType().c_str(), "flat")) {
                                            integrator = new FlatIntegrator();
                                        } else if (std::strcmp(material->getType().c_str(), "depth")) {
                                            integrator = new DepthMapIntegrator();
                                        } else if (std::strcmp(material->getType().c_str(), "normal")) {
                                            integrator = new NormalMapIntegrator();
                                        }
                                    }else {
                                        material->setType("flat");
                                    }
                                    if(materialElement->Attribute("color")) {
                                        auto aux = materialElement->Attribute("color");
                                        color c(0, 0, 0);
                                        char *token = std::strtok(const_cast<char *>(aux), " ");
                                        int i = 0;
                                        while (token != nullptr) {
                                            try {
                                                c[i] = std::stoi(token);
                                                token = std::strtok(nullptr, " ");
                                                i++;
                                            } catch (std::exception &e) {
                                                std::cout << "Failed to retrieve color vector. Setting to default value"
                                                          << std::endl;
                                            }
                                        }
                                        material->setCol(c);
                                    }
                                    if(materialElement->FirstChildElement("object")){
                                        auto object = materialElement->FirstChildElement("object");
                                        do{
                                            std::string objectType;
                                            if(object->Attribute("type")){
                                                objectType = object->Attribute("type");
                                            }else{
                                                std::cout << "Object type not defined. Creating sphere." << std::endl;
                                                objectType = "sphere";
                                            }
                                            if(std::strcmp(objectType.c_str(),"sphere") == 0){
                                                double radius = 1;
                                                point3 center(0,0,0);
                                                if(object->Attribute("radius"))
                                                    radius = std::stod(object->Attribute("radius"));
                                                else
                                                    std::cout << "No radius specified. Setting to default." << std::endl;
                                                if(object->Attribute("center")){
                                                    auto aux = object->Attribute("center");
                                                    point3 p(0, 0, 0);
                                                    auto token = std::strtok(const_cast<char *>(aux), " ");
                                                    auto i = 0;
                                                    while (token != nullptr) {
                                                        try {
                                                            p[i] = std::stod(token);
                                                            token = std::strtok(nullptr, " ");
                                                            i++;
                                                        } catch (std::exception &e) {
                                                            std::cout << "Failed to retrieve sphere center. Setting to default value"
                                                                      << std::endl;
                                                        }
                                                    }
                                                    center = p;
                                                }else{
                                                    std::cout << "Failed to retrieve sphere center. Setting to default values"
                                                              << std::endl;
                                                }
                                                auto* s = new Sphere(center,radius,integrator);
                                                s->setMaterial(*material);
                                                ro.objects.push_back(s);
                                            }
                                            object = object->NextSiblingElement();
                                        }while(object != nullptr);
                                    }
                                    materialElement = materialElement->NextSiblingElement();
                                }while(materialElement != nullptr);

                            }else{
                               /* //Objects
                                if(RT3->FirstChildElement("object")){
                                    auto object = RT3->FirstChildElement("object");
                                    while(object->NextSiblingElement() != nullptr){
                                        std::string objectType;
                                        if(object->Attribute("type")){
                                            objectType = object->Attribute("type");
                                        }else{
                                            std::cout << "Object type not defined. Creating sphere." << std::endl;
                                            objectType = "sphere";
                                        }
                                        if(std::strcmp(objectType.c_str(),"sphere") == 0){
                                            double radius = 1;
                                            point3 center(0,0,0);
                                            if(object->Attribute("radius"))
                                                radius = std::stod(object->Attribute("radius"));
                                            else
                                                std::cout << "No radius specified. Setting to default." << std::endl;
                                            if(object->Attribute("center")){
                                                std::string aux = object->Attribute("center");
                                                point3 p(0, 0, 0);
                                                char *token = std::strtok(const_cast<char *>(aux.c_str()), " ");
                                                int i = 0;
                                                while (token != nullptr) {
                                                    try {
                                                        p[i] = std::stod(token);
                                                        token = std::strtok(nullptr, " ");
                                                        i++;
                                                    } catch (std::exception &e) {
                                                        std::cout << "Failed to retrieve sphere center. Setting to default value"
                                                                  << std::endl;
                                                    }
                                                }
                                                center = p;
                                            }else{
                                                std::cout << "Failed to retrieve sphere center. Setting to default values"
                                                          << std::endl;
                                            }
                                            auto* s = new Sphere(center,radius,ro.materialColor);
                                            ro.objects.push_back(s);
                                        }
                                        object = object->NextSiblingElement();
                                    }
                                }*/
                            }
                            /*if(RT3->FirstChildElement("material")){
                                if(RT3->FirstChildElement("material")->Attribute("type"))
                                    ro.materialType = RT3->FirstChildElement("material")->Attribute("type");
                                else
                                    std::cout << "No material type retrieved. Setting to default" << std::endl;
                                if(RT3->FirstChildElement("material")->Attribute("color")) {
                                    std::string aux = RT3->FirstChildElement("material")->Attribute("color");
                                    color c(0, 0, 0);
                                    char *token = std::strtok(const_cast<char *>(aux.c_str()), " ");
                                    int i = 0;
                                    while (token != nullptr) {
                                        try {
                                            c[i] = std::stoi(token);
                                            token = std::strtok(nullptr, " ");
                                            i++;
                                        } catch (std::exception &e) {
                                            std::cout << "Failed to retrieve color vector. Setting to default value"
                                                      << std::endl;
                                        }
                                    }
                                    ro.materialColor = c;
                                }
                            }
                            //Objects
                            if(RT3->FirstChildElement("object")){
                                auto object = RT3->FirstChildElement("object");
                                while(object->NextSiblingElement() != nullptr){
                                    std::string objectType;
                                    if(object->Attribute("type")){
                                        objectType = object->Attribute("type");
                                    }else{
                                        std::cout << "Object type not defined. Creating sphere." << std::endl;
                                        objectType = "sphere";
                                    }
                                    if(std::strcmp(objectType.c_str(),"sphere") == 0){
                                        double radius = 1;
                                        point3 center(0,0,0);
                                        if(object->Attribute("radius"))
                                            radius = std::stod(object->Attribute("radius"));
                                        else
                                            std::cout << "No radius specified. Setting to default." << std::endl;
                                        if(object->Attribute("center")){
                                            std::string aux = object->Attribute("center");
                                            point3 p(0, 0, 0);
                                            char *token = std::strtok(const_cast<char *>(aux.c_str()), " ");
                                            int i = 0;
                                            while (token != nullptr) {
                                                try {
                                                    p[i] = std::stod(token);
                                                    token = std::strtok(nullptr, " ");
                                                    i++;
                                                } catch (std::exception &e) {
                                                    std::cout << "Failed to retrieve sphere center. Setting to default value"
                                                              << std::endl;
                                                }
                                            }
                                            center = p;
                                        }else{
                                            std::cout << "Failed to retrieve sphere center. Setting to default values"
                                                      << std::endl;
                                        }
                                        auto* s = new Sphere(center,radius,ro.materialColor);
                                        ro.objects.push_back(s);
                                    }
                                    object = object->NextSiblingElement();
                                }
                            }*/
                            return ro;
                        }else{
                            throw std::invalid_argument("Unable to open Scene File");
                        }
                    }
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
                default:
                    break;
            }
        }
    }

    static void toString(const struct RunningOptions& ro){
        std::cout << "cameraType;" << ro.cameraType << "\n" <<
                     "filmType;\n" << ro.filmType << "\n" <<
                     "filmX_res;\n"<< ro.filmX_res << "\n" <<
                     "filmY_res;\n"<< ro.filmY_res << "\n" <<
                     "filmFilename;\n"<< ro.filmFilename << "\n" <<
                     "filmImgtype;\n"<< ro.filmImgtype << "\n" <<
                     "backgroundType;\n"<< ro.backgroundType << "\n" <<
                     "backgroundMapping;\n"<< ro.backgroundMapping << "\n" <<
                     "backgroundColor;\n"<< ro.backgroundColor << "\n" <<
                     "backgroundBl;\n"<< ro.backgroundBl << "\n" <<
                     "backgroundBr;\n"<< ro.backgroundBr << "\n" <<
                     "backgroundTl;\n"<< ro.backgroundTl << "\n" <<
                     "backgroundTr;"<< ro.backgroundTr << "\n" << std::endl;
    }
};

#endif //RT_PARCER_H
