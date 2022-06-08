#include "../includes/Vector/color.h"
#include "../includes/Vector/ray.h"
#include "../includes/Vector/vec3.h"
#include "../includes/RTUtilities/parcer.h"
#include "../includes/RTEngine/primitive.h"
#include "../includes/RTEngine/sphere.h"
#include "../includes/RTEngine/Integrator.h"
#include "../includes/RTEngine/DepthMapIntegrator.h"
#include "../includes/RTUtilities/RunningOptions.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    struct RunningOptions ro;
    FlatIntegrator fi;
    try {
        ro = Parser::parse(argc, argv);
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
        return -1;
    }
    if(ro.help){
        return 0;
    }

    // Image
    const int image_width = ro.filmX_res;
    const int image_height = ro.filmY_res;
    const auto aspect_ratio = static_cast<double>(image_width)/static_cast<double>(image_height);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = -1;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render
    std::ofstream file;
    file.open(ro.filmFilename+'.'+ro.filmImgtype);
    file << "P3\n" << image_width << " " << image_height << "\n255\n";
    if(std::strcmp(ro.cameraType.c_str(),"perspective") == 0) {
        for (int j = image_height - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                auto u = double(i) / (image_width - 1);
                auto v = double(j) / (image_height - 1);
                ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
                color pixel_color = fi.ray_color(r, ro);
                write_color(file, pixel_color);
            }
        }
    }else if(std::strcmp(ro.cameraType.c_str(),"orthographic")==0){
        for (int j = image_height - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                auto u = double(i) / (image_width - 1);
                auto v = double(j) / (image_height - 1);
                ray r(lower_left_corner + u * horizontal + v * vertical - origin, point3(0, 0, 1));
                color pixel_color = fi.ray_color(r, ro);
                write_color(file, pixel_color);
            }
        }
    }else{
        std::cout << "Invalid camera setting. Aborting" << std::endl;
    }
    file.close();

    std::cerr << "\nDone.\n";
    return 0;
}