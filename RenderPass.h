#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class RenderPass
{
public:
	RenderPass();
	~RenderPass();

	void beginRenderPass(uint32_t height, uint32_t width, VkFramebuffer framebuffer, VkCommandBuffer commandBuffer);
	virtual void create(LogicalDevice logicalDevice, VkFormat imageFormat) = 0;
	void destroy(LogicalDevice logicalDevice);
	VkRenderPass getRenderPass();

protected:
	VkRenderPass renderPass;
};