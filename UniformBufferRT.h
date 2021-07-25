#pragma once
#include "UniformBufferObject.h"

#include <glm/glm.hpp>

struct RT {
    alignas(16) glm::mat4 viewInverse;
    alignas(16) glm::mat4 projInverse;
    alignas(16) glm::vec3 lightPos;
    alignas(16) uint32_t vertexSize;
};

class UniformBufferRT : public UniformBufferObject
{
public:
	UniformBufferRT();
	~UniformBufferRT();

    void update(LogicalDevice& logicalDevice, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPosition, uint32_t vertexSize);
};