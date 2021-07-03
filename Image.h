#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class Image
{
public:
	Image();
	~Image();

	void create(LogicalDevice logicalDevice,
		uint32_t height,
		uint32_t width,
		VkFormat format,
		VkImageUsageFlagBits usage,
		VkSampleCountFlagBits samples,
		uint32_t arrayLayers,
		VkImageCreateFlags flags,
		bool genMipLevels);
	void destroy(LogicalDevice logicalDevice);
	VkImage getImage();

private:
	uint32_t arrayLayers;
	VkFormat format;
	VkImage image;
	VkDeviceMemory imageMemory;
	uint32_t mipLevels;

	void createImage(LogicalDevice logicalDevice,
		uint32_t width,
		uint32_t height,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProperties,
		VkSampleCountFlagBits samples,
		VkImageCreateFlags flags);
};