#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "parcer.h"
#include "primitive.h"
#include "sphere.h"

#include <iostream>
#include <fstream>

color ray_color(const ray& r, const std::vector<Primitive*>& shapes, color tr, color tl, color br, color bl) {
    for(auto shape : shapes){
        if(shape->intersects(r)){
            return shape->getColor();
        }
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    color c1 = (1.0-t)*bl + t*tr;
    color c2 = (1.0-t)*br + t*tl;
    auto h = 0.5*(unit_direction.x() + 1.0);
    return (1.0-h)*c1 + h*c2;
}

int main(int argc, char* argv[]) {
    struct RunningOptions ro;
    try {
        ro = Parser::parse(argc, argv);
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
        return -1;
    }
    if(ro.help){
        return 0;
    }
    //Scene
    std::vector<Primitive*> shapes;
    auto *s1 = new Sphere(point3(1,-1,-1),1,color(0,0,255));
    auto *s2 = new Sphere(point3(2,4,-6),2,color(255,0,0));
    auto *s3 = new Sphere(point3(1,1,-10), 3,color(0,255,0));
    shapes.push_back(s1);
    shapes.push_back(s2);
    shapes.push_back(s3);

    // Image
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 1920;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render
    std::ofstream file;
    file.open(ro.filmFilename+'.'+ro.filmImgtype);
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r, shapes, ro.backgroundTr/255,ro.backgroundTl/255,ro.backgroundBr/255,ro.backgroundBl/255);
            write_color(file, pixel_color);
        }
    }
    file.close();

    std::cerr << "\nDone.\n";
}