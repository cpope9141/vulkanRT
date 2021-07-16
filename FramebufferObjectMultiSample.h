#pragma once
#include "FramebufferObject.h"

#include "Image.h"
#include "ImageView.h"

class FramebufferObjectMultiSample : public FramebufferObject
{
public:
	FramebufferObjectMultiSample();
	FramebufferObjectMultiSample(uint32_t height, uint32_t width, VkFormat colorFormat);
	~FramebufferObjectMultiSample();

	virtual void create(LogicalDevice logicalDevice, RenderPass* renderPass) override;
	virtual void destroy(LogicalDevice logicalDevice) override;

private:
	Image msImage;
	ImageView msImageView;
};