//
// Created by Gabriel on 18/07/2022.
//

#ifndef RT_FILM_H
#define RT_FILM_H

#include <string>
#include <vector>
#include <fstream>
#include "../../Vector/vec3.h"

class Film
{
private:
    /* data */
    std::string filename;
    std::string img_type;
    std::string type;
    int x_res;
    int y_res;
    std::vector<std::vector<vec3>> output;

public:
    Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res);
    ~Film();
    int width();
    int height();
    void fillEmpty();
    void add(int x, int y, vec3 color);
    void write_image();
};

void Film::fillEmpty(){
    for(int i =0; i < x_res; i++){
        std::vector<vec3> vector;
        for(int j=0; j < y_res; j++){
            vector.emplace_back(0.0, 0.0, 0.0);
        }
        output.push_back(vector);
    }
}

Film::Film(std::string filename, std::string img_type, std::string type, int x_res, int y_res)
{
    this->filename = filename;
    this->img_type = img_type;
    this->type = type;
    this->x_res = x_res;
    this->y_res = y_res;

    fillEmpty();
}

Film::~Film() = default;

int Film::width(){
    return x_res;
}

int Film::height(){
    return y_res;
}

void Film::add(int x, int y, vec3 color){
    output[x][y] = color;
}

void Film::write_image(){
    if (img_type == "ppm") {
        std::ofstream outfile;
        outfile.open(filename);
        outfile << "P3" << std::endl;
        outfile << width() << " " << height() << std::endl;
        outfile << "255" << std::endl;
        for(int i = height() - 1; i >= 0; i--){
            for(int j = 0; j < width(); j++){
                outfile << output[j][i]*255 << std::endl;
            }
        }
        outfile.close();
    } else if(img_type == "png"){
        std::vector<unsigned char> image;
        image.resize(width() * height() * 4);
        int o = 0;
        for(int i = height() - 1; i >= 0; i--){
            for(int j = 0; j < width(); j++){
                image[o] = (int) (output[j][i].e[0] * 255);
                image[o+1] = (int) (output[j][i].e[1] * 255);
                image[o+2] = (int) (output[j][i].e[2] * 255);
                image[o+3] = 255;
                o+=4;
            }
        }
    }
}

#endif //RT_FILM_H
