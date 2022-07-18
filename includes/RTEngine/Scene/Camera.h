//
// Created by Gabriel on 18/07/2022.
//

#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include <cmath>
#include <string>
#include "../../Vector/vec3.h"
#include "../../Vector/ray.h"

class Camera
{
private:

public:

    virtual Ray generate_ray(int x, int y) {}
    virtual Ray generate_ray(float x, float y) {}

    std::string type;

    float l;
    float r;
    float b;
    float t;

    float fovy = 0;

    vec3 gaze;
    vec3 w;
    vec3 u;
    vec3 v;
    vec3 e;

    float u_;
    float v_;

    int height;
    int width;

    Camera(std::string type, std::string screen, vec3 lookat, vec3 lookfrom, vec3 vup) {}
    Camera(std::string type, float fovy, vec3 lookat, vec3 lookfrom, vec3 vup) {}
    void initializeScreen(std::string screen);
    void initializeFrame(vec3 lookat, vec3 lookfrom, vec3 vup);
    void setHeightWidth(int height, int width);
    virtual ~Camera() = default;
};

void Camera::initializeScreen(std::string screen){
    if(screen != ""){
        std::istringstream iss(screen);
        std::vec3tor<std::string> splited((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());

        std::istringstream(splited[0]) >> l;
        std::istringstream(splited[1]) >> r;
        std::istringstream(splited[2]) >> b;
        std::istringstream(splited[3]) >> t;

        std::cout << l << " " << r << " " << b << " "  << t << std::endl;
    }
}

void Camera::initializeFrame(vec3 lookat, vec3 lookfrom, vec3 vup){
    this->gaze = lookat - lookfrom;
    this->w = normalize(gaze);
    this->u = normalize(cross(vup, w));
    this->v = normalize(cross(w, u));
    this->e = lookfrom;
}

void Camera::setHeightWidth(int height, int width){

    this->height = height;
    this->width = width;

    if(fovy > 0){
        float ratio = (float) width/ (float) height;
        float h = tan(((fovy*M_PI)/180.0)/2.0);

        // h /= 2;
        this->l = -ratio * h;
        this->r = ratio * h;
        this->t = h;
        this->b = -h;

        //std::cout << l << " " << r << " " << t << " " << b << " " << ratio << " " << h << std::endl;
    }
}

class Perspectivec3amera : public Camera{

public:

    Perspectivec3amera(std::string type, float fovy, vec3 lookat, vec3 lookfrom, vec3 vup) : Camera(type, fovy, lookat, lookfrom, vup){
        this->type = type;
        this->fovy = fovy;
        initializeFrame(lookat, lookfrom, vup);
    }

    ray generate_ray(int x, int y){
        u_ = l + (r - l) * ((x+0.5)/ (float) width);
        v_ = b + (t - b) * ((y+0.5)/ (float) height);
        //std::cout << u_ << " " << v_ << std::endl;
        ray r(e, normalize(w + u*u_ + v*v_));
        return r;
    }

    ray generate_ray(float x, float y){
        u_ = l + (r - l) * ((y+0.5)/ (float) width);
        v_ = b + (t - b) * ((x+0.5)/ (float) height);

        //std::cout << u_ << " " << v_ << std::endl;
        ray r(e, normalize(w + u* (float) u_ + v* (float) v_));
        return r;
    }

    virtual ~Perspectivecamera() = default;
};

class OrthograficCamera : public Camera{
public:

    OrthograficCamera(std::string type, std::string screen, vec3 lookat, vec3 lookfrom, vec3 vup) : Camera(type, screen, lookat, lookfrom, vup){
        this->type = type;
        initializeScreen(screen);
        initializeFrame(lookat, lookfrom, vup);
    }

    ray generate_ray(int x, int y){
        u_ = l + (r - l) * ((x+0.5)/ (float) width);
        v_ = b + (t - b) * ((y+0.5)/ (float) height);
        //std::cout  << u_ << " " << v_ << std::endl;
        ray r(e + u* (float) u_ + v* (float) v_, w);
        return r;

    }

    ray generate_ray(float x, float y){
        u_ = l + (r - l) * ((x+0.5)/ (float) width);
        v_ = b + (t - b) * ((y+0.5)/ (float) height);
        ray r(e + u* (float) u_ + v* (float) v_, w);
        return r;
    }

    virtual ~OrthograficCamera() = default;
};
#endif //RT_CAMERA_H
