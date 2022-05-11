//
// Created by gabriel on 4/28/22.
//

#ifndef RT_PARCER_H
#define RT_PARCER_H

#include <string>
#include <sstream>
#include "tinyxml2.h"

struct RunningOptions{
    bool  help = false;
    std::string cameraType;
    std::string filmType;
    int filmX_res = 0;
    int filmY_res = 0;
    std::string filmFilename;
    std::string filmImgtype;
    std::string backgroundType;
    std::string backgroundMapping;
    color backgroundColor = color(0,0,0);
    color backgroundBl = color(0,0,0);
    color backgroundBr = color(0,0,0);
    color backgroundTl = color(0,0,0);
    color backgroundTr = color(0,0,0);
} RunningOptions;

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
                            ro.backgroundMapping = background->Attribute("mapping");
                            if (background->Attribute("color")) {
                                std::string aux = background->Attribute("color");
                                color c(0,0,0);
                                std::stringstream aux2;
                                for(int i = 0; i < 2; i++){
                                    aux2 << aux;
                                    try {
                                        c[i] = std::stoi(aux2.str());
                                    }catch(std::exception& e){
                                        std::cout << "Failed to convert color vector. Setting default value" << std::endl;
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
                                std::stringstream sAuxBl, sAuxBr,sAuxTl,sAuxTr;
                                for(int i = 0; i < 2; i++){
                                    sAuxBl << auxbl;
                                    sAuxBr << auxbr;
                                    sAuxTl << auxtl;
                                    sAuxTr << auxtr;
                                    try {
                                        bl[i] = std::stoi(sAuxBl.str());
                                        br[i] = std::stoi(sAuxBr.str());
                                        tl[i] = std::stoi(sAuxTl.str());
                                        tr[i] = std::stoi(sAuxTr.str());
                                    }catch(std::exception& e){
                                        std::cout << "Failed to convert color vectors. Setting default value" << std::endl;
                                    }
                                }
                                ro.backgroundBl = bl;
                                ro.backgroundBr = br;
                                ro.backgroundTl = tl;
                                ro.backgroundTr = tr;
                            }
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
