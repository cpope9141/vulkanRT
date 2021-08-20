#pragma once
#include "CommandPool.h"
#include "Image.h"
#include "ImageView.h"
#include "RGBAResource.h"
#include "Sampler.h"
#include "vulkan/vulkan_core.h"

class Texture
{
public:
	Texture();
	Texture(VkFilter filter);
	~Texture();

	void create(const LogicalDevice& logicalDevice,
		uint32_t height,
		uint32_t width,
		VkFormat format,
		VkImageAspectFlags imageAspect,
		VkImageUsageFlags usage,
		VkSampleCountFlagBits sampleCount,
		bool enableAnisotropy,
		VkBorderColor borderColor);
	void create(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const RGBAResource& resource);
	void destroy(const LogicalDevice& logicalDevice);
	uint32_t getHeight();
	Image getImage();
	VkImageView getImageView();
	VkSampler getSampler();
	uint32_t getWidth();

protected:
	VkFilter filter;
	uint32_t height;
	Image image;
	ImageView imageView;
	Sampler sampler;
	uint32_t width;
};