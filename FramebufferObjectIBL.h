#pragma once
#include "FramebufferObject.h"

const uint32_t FBO_IBL_IRRADIANCE_EXTENT = 128;
const uint32_t FBO_IBL_PREFILTERED_EXTENT = 512;
const uint32_t FBO_IBL_INTEGRATED_BRDF_EXTENT = 512;

class FramebufferObjectIBL : public FramebufferObject
{
public:
	FramebufferObjectIBL(VkFormat colorFormat, uint32_t height, uint32_t width);
	~FramebufferObjectIBL();

	virtual void create(const LogicalDevice& logicalDevice, RenderPass* renderPass) override;
};