#pragma once
#include "UniformBufferObject.h"

#include "LogicalDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>

const uint8_t TYPE_ORTHOGRAPHIC = 0;
const uint8_t TYPE_PERSPECTIVE = 1;

class UniformBufferProjection : public UniformBufferObject
{
public:
    UniformBufferProjection(uint8_t type);
    ~UniformBufferProjection();

    glm::mat4 getProjection();
    void update(const LogicalDevice& logicalDevice, float height, float width, float near, float far);

private:
    glm::mat4 projection;
    uint8_t type;
};