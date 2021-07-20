#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>

struct CubeVertex
{
    glm::vec3 position;
    glm::vec2 texelCoords;
};

struct QuadVertex
{
	glm::vec2 position;
	glm::vec2 texelCoords;
};

struct StaticMeshVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texelCoords;
};

struct ModelResource
{
    std::vector<uint32_t> indices;
    std::vector<StaticMeshVertex> vertices;
};

struct HDRResource
{
    int height;
    float* imageHDR;
    int width;
};