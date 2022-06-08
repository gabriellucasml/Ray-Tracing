cmake_minimum_required(VERSION 3.22)
project(RT)

set(CMAKE_CXX_STANDARD 14)

add_executable(RT source/main.cpp includes/Vector/vec3.h includes/Vector/color.h includes/Vector/ray.h "includes/RTUtilities/parcer.h" source/tinyxml2.cpp includes/RTUtilities/tinyxml2.h includes/RTEngine/primitive.h includes/RTEngine/sphere.h includes/RTEngine/Integrator.h includes/RTEngine/FlatIntegrator.h includes/RTEngine/DepthMapIntegrator.h includes/RTEngine/NormalMapIntegrator.h includes/RTEngine/Material.h includes/RTUtilities/RunningOptions.h)
