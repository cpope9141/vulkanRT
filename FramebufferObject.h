#pragma once
#include "Framebuffer.h"
#include "RenderPass.h"
#include "Texture.h"
#include "vulkan/vulkan_core.h"

class FramebufferObject
{
public:
	FramebufferObject();
	FramebufferObject(uint32_t height, uint32_t width, VkFormat colorFormat);
	~FramebufferObject();

	virtual void create(const LogicalDevice& logicalDevice, RenderPass* renderPass);
	virtual void destroy(const LogicalDevice& logicalDevice);
	Texture getColorAttachment();
    Texture getDepthAttachment();
	VkFramebuffer getFramebuffer();
	uint32_t getHeight();
	uint32_t getWidth();

protected:
	Texture colorAttachment = Texture(VK_FILTER_LINEAR);
	VkFormat colorFormat;
	Texture depthAttachment = Texture(VK_FILTER_LINEAR);
	Framebuffer framebuffer;
	uint32_t height;
	uint32_t width;
};