#pragma once
#include "UniformBufferObject.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>

class UniformBufferStaticModelPBR : public UniformBufferObject
{
public:
	UniformBufferStaticModelPBR();
	~UniformBufferStaticModelPBR();

	void update(LogicalDevice logicalDevice, glm::mat4 viewMatrix, glm::mat4 modelMatrix, glm::mat4 normalMatrix);
};