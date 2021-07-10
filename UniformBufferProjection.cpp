#include "UniformBufferProjection.h"

#include "WindowManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

//public
UniformBufferProjection::UniformBufferProjection(uint8_t type)
{
	size = sizeof(glm::mat4);
	this->type = type;
}

UniformBufferProjection::~UniformBufferProjection() {}

glm::mat4 UniformBufferProjection::getProjection() { return projection; }

void UniformBufferProjection::update(LogicalDevice logicalDevice, float height, float width, float near, float far)
{
    void* ptr = nullptr;
    vkMapMemory(logicalDevice.getDevice(), bufferMemory, 0, getSize(), 0, &ptr);

    if (TYPE_ORTHOGRAPHIC == type)
    {
        const float TOP = HEIGHT / 2;
        const float BOTTOM = -HEIGHT / 2;
        const float RIGHT = WIDTH / 2;
        const float LEFT = -WIDTH / 2;
        const float FAR = WIDTH / 2;
        const float NEAR = -WIDTH / 2;

        projection = glm::ortho(LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR);
    }
    else if (TYPE_PERSPECTIVE == type)
    {
        projection = glm::perspective(glm::radians(45.0f), width / height, near, far);
    }
    else
    {
        throw std::runtime_error("Invalid projection type");
    }

    projection[1][1] *= -1;
    memcpy(ptr, &projection, sizeof(projection));

    vkUnmapMemory(logicalDevice.getDevice(), bufferMemory);
}