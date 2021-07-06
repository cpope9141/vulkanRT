#pragma once
#include "Image.h"
#include "ImageView.h"
#include "Sampler.h"
#include "vulkan/vulkan_core.h"

class Texture
{
public:
	Texture(VkFilter filter);
	~Texture();

	void create(LogicalDevice logicalDevice,
		uint32_t height,
		uint32_t width,
		VkFormat format,
		VkImageAspectFlags imageAspect,
		VkImageUsageFlagBits usage,
		VkSampleCountFlagBits sampleCount,
		bool enableAnisotropy,
		VkBorderColor borderColor);
	void destroy(LogicalDevice logicalDevice);
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