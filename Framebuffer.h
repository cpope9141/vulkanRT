#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void create(const LogicalDevice& logicalDevice, VkRenderPass renderPass, uint32_t height, uint32_t width, VkImageView imageView);
	void create(const LogicalDevice& logicalDevice, VkRenderPass renderPass, uint32_t height, uint32_t width, VkImageView imageView, VkImageView depthView, VkImageView resolveView);
	void destroy(const LogicalDevice& logicalDevice);
	VkFramebuffer getFramebuffer();

private:
	VkFramebuffer framebuffer;
};