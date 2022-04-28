//
// Created by gabriel on 4/28/22.
//

#ifndef RT_PARCER_H
#define RT_PARCER_H

#include <string>
#include "tinyxml2.h"

struct RunningOptions{
    std::string cameraType;
    std::string filmType;
    std::string filmX_res;
    std::string filmY_res;
    std::string filmFilename;
    std::string filmImgtype;
    std::string backgroundType;
    std::string backgroundMapping;
    std::string backgroundColor;
    std::string backgroundBl;
    std::string backgroundBr;
    std::string backgroundTl;
    std::string backgroundTr;
} RunningOptions;

class Parser{
public:
    static struct RunningOptions parse(int argc, char* argv[]){
        if(argc < 2 or argc > 8){
            throw std::exception();
        }else{
            struct RunningOptions ro;
            switch(argc){
                case 2:
                    if(strcmp(argv[1],"--help") == 0 or strcmp(argv[1],"--quick") == 0 or strcmp(argv[1],"--cropWindow") == 0 or strcmp(argv[1], "--outfile") == 0){
                        throw std::exception();
                    }else{
                        tinyxml2::XMLDocument doc;
                        doc.LoadFile(argv[1]);
                        auto RT3 = doc.RootElement();

                        ro.cameraType = RT3->FirstChildElement("camera")->Attribute("type");

                        ro.filmType = RT3->FirstChildElement("film")->Attribute("type");
                        ro.filmX_res = RT3->FirstChildElement("film")->Attribute("x_res");
                        ro.filmY_res = RT3->FirstChildElement("film")->Attribute("y_res");
                        ro.filmFilename = RT3->FirstChildElement("film")->Attribute("filename");
                        ro.filmImgtype = RT3->FirstChildElement("film")->Attribute("img_type");

                        auto background = RT3->FirstChildElement("background");
                        ro.backgroundType = background->Attribute("type");
                        ro.backgroundMapping = background->Attribute("mapping");
                        if(background->Attribute("color")){
                            ro.backgroundColor = background->Attribute("color");
                            ro.backgroundBl = background->Attribute("color");
                            ro.backgroundBr = background->Attribute("color");
                            ro.backgroundTl = background->Attribute("color");
                            ro.backgroundTr = background->Attribute("color");
                        }else {
                            ro.backgroundBl = background->Attribute("bl");
                            ro.backgroundBr = background->Attribute("br");
                            ro.backgroundTl = background->Attribute("tl");
                            ro.backgroundTr = background->Attribute("tr");
                        }
                        return ro;
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
