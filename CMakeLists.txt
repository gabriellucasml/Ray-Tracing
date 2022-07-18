cmake_minimum_required(VERSION 3.22)
project(RT)

set(CMAKE_CXX_STANDARD 14)

add_executable(RT source/main.cpp includes/Vector/vec3.h includes/Vector/color.h includes/Vector/ray.h "includes/RTUtilities/parcer.h" source/tinyxml2.cpp includes/RTUtilities/tinyxml2.h includes/RTEngine/Objects/primitive.h includes/RTEngine/Objects/sphere.h includes/RTEngine/Integrators/Integrator.h includes/RTEngine/Integrators/FlatIntegrator.h includes/RTEngine/Integrators/DepthMapIntegrator.h includes/RTEngine/Materials/NormalMapIntegrator.h includes/RTEngine/Materials/Material.h includes/RTUtilities/RunningOptions.h includes/RTEngine/Materials/BlinnPhongMaterial.h includes/RTEngine/Integrators/BlinnPhongIntegrator.h includes/RTEngine/Lights/ambient.hpp includes/RTEngine/Lights/directional.hpp includes/RTEngine/Lights/light.hpp includes/RTEngine/Lights/ambient.hpp includes/RTEngine/Lights/point.hpp includes/RTEngine/Lights/spot.hpp includes/RTEngine/Objects/Surfel.h includes/RTEngine/Objects/bounds3.h includes/RTEngine/Scene/Scene.h includes/RTEngine/Scene/Camera.h includes/RTEngine/Scene/Background.h includes/RTEngine/Scene/Film.h includes/RTEngine/Scene/Ambient.h)

