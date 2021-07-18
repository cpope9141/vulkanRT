#pragma once
#include "FramebufferObject.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

const glm::vec2 FBO_IBL_IRRADIANCE_EXTENT = glm::vec2(128, 128);

class FramebufferObjectIBL : public FramebufferObject
{
public:
	FramebufferObjectIBL(VkFormat colorFormat, uint32_t height, uint32_t width);
	~FramebufferObjectIBL();

	virtual void create(LogicalDevice logicalDevice, RenderPass* renderPass) override;
};