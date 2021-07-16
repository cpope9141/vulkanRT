#pragma once
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "LogicalDevice.h"
#include "RGBAResource.h"
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
		VkImageUsageFlags usage,
		VkSampleCountFlagBits samples,
		uint32_t arrayLayers,
		VkImageCreateFlags flags,
		bool genMipLevels);
	void create(LogicalDevice logicalDevice, CommandPool commandPool, const RGBAResource& resource);
	void destroy(LogicalDevice logicalDevice);
	VkImage getImage();
	uint32_t getMipLevels();
	void transitionLayout(LogicalDevice logicalDevice, CommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout, bool submit);

private:
	uint32_t arrayLayers;
	VkFormat format;
	VkImage image;
	VkDeviceMemory imageMemory;
	uint32_t mipLevels;

	void copyBufferToImage(LogicalDevice logicalDevice, CommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, size_t regionCount, VkDeviceSize bytes);
	void createImage(LogicalDevice logicalDevice,
		uint32_t width,
		uint32_t height,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProperties,
		VkSampleCountFlagBits samples,
		VkImageCreateFlags flags);
	void generateMipmaps(LogicalDevice logicalDevice, CommandPool commandPool, VkFormat imageFormat, int width, int height, uint32_t mipLevels);
	bool hasStencilComponent(VkFormat format);
};