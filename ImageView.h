#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class ImageView
{
public:
	ImageView();
	~ImageView();

	void create(LogicalDevice logicalDevice, VkImage image, VkImageViewType viewType, uint32_t mipLevels, VkFormat format, VkImageAspectFlags imageAspect, uint32_t layerCount);
	void destroy(const LogicalDevice& logicalDevice);
	VkImageView getImageView();

private:
	VkImageView imageView;
};