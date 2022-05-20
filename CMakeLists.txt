cmake_minimum_required(VERSION 3.22)
project(RT)

set(CMAKE_CXX_STANDARD 14)

add_executable(RT main.cpp vec3.h color.h ray.h "parcer.h" tinyxml2.cpp tinyxml2.h primitive.h sphere.h)
