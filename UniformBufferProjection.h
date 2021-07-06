#pragma once
#include "UniformBufferObject.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include "LogicalDevice.h"

const uint8_t TYPE_ORTHOGRAPHIC = 0;
const uint8_t TYPE_PERSPECTIVE = 1;

class UniformBufferProjection : public UniformBufferObject
{
public:
    UniformBufferProjection(uint8_t type);
    ~UniformBufferProjection();

    glm::mat4 getProjection();
    void update(LogicalDevice logicalDevice, float height, float width, float near, float far);

private:
    glm::mat4 projection;
    uint8_t type;
};