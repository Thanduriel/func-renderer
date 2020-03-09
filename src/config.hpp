#pragma once

#include <glm.hpp>

//#define MODE2D
//#define MULTITHREADED
//#define MODE2D
constexpr unsigned c_maxNumThreads = 4;

#ifdef MODE2D
const glm::vec3 c_cameraLocation = glm::vec3(0.f, 0.f, -2.f);
const glm::vec3 c_cameraOrigin = glm::vec3(0.f, 0.f, 0.f);
#else
const glm::vec3 c_cameraLocation = glm::vec3(0.f, 10.f, 0.f);
const glm::vec3 c_cameraOrigin = glm::vec3(10.f, 0.f, 10.f);
#endif

#ifndef NDEBUG
constexpr float c_worldSize = 20.f;
#else
constexpr float c_worldSize = 100.f;
#endif

constexpr const char* c_shaderPath = "../shader/";
constexpr const char* c_texturePath = "../texture/";