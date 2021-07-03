#include "Image.h"

#include "MemoryUtilities.h"

#include <cmath>
#include <iostream>

//public
Image::Image()
{
	arrayLayers = 1;
	format = VK_FORMAT_UNDEFINED;
	image = VK_NULL_HANDLE;
	imageMemory = VK_NULL_HANDLE;
	mipLevels = 1;
}

Image::~Image() {}

void Image::create(LogicalDevice logicalDevice,
	uint32_t height,
	uint32_t width,
	VkFormat format,
	VkImageUsageFlagBits usage,
	VkSampleCountFlagBits samples,
	uint32_t arrayLayers,
	VkImageCreateFlags flags,
	bool genMipLevels)
{
	this->arrayLayers = arrayLayers;
	this->format = format;

	if (genMipLevels)
	{
		mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
	}

	createImage(logicalDevice,
		width,
		height,
		VK_IMAGE_TILING_OPTIMAL,
		usage,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		samples,
		flags);
}

void Image::destroy(LogicalDevice logicalDevice)
{
	vkDestroyImage(logicalDevice.getDevice(), image, nullptr);
	vkFreeMemory(logicalDevice.getDevice(), imageMemory, nullptr);
}

VkImage Image::getImage() { return image; }

//private
void Image::createImage(LogicalDevice logicalDevice,
	uint32_t width,
	uint32_t height,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags memProperties,
	VkSampleCountFlagBits samples,
	VkImageCreateFlags flags)
{
	VkImageCreateInfo imageCreateInfo = {};

	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = mipLevels;
	imageCreateInfo.arrayLayers = arrayLayers;
	imageCreateInfo.format = format;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = usage;
	imageCreateInfo.samples = samples;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.flags = flags;

	if (VK_SUCCESS == vkCreateImage(logicalDevice.getDevice(), &imageCreateInfo, nullptr, &image))
	{
		VkMemoryRequirements memRequirements = {};
		VkMemoryAllocateInfo allocInfo = {};
		vkGetImageMemoryRequirements(logicalDevice.getDevice(), image, &memRequirements);

		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(logicalDevice, memRequirements.memoryTypeBits, memProperties);

		if (VK_SUCCESS == vkAllocateMemory(logicalDevice.getDevice(), &allocInfo, nullptr, &imageMemory))
		{
			if (VK_SUCCESS != vkBindImageMemory(logicalDevice.getDevice(), image, imageMemory, 0))
			{
				throw std::runtime_error("Failed to bind image memory");
			}
		}
		else
		{
			throw std::runtime_error("Failed to allocate image memory");
		}
	}
	else
	{
		throw std::runtime_error("Failed to create image");
	}
}