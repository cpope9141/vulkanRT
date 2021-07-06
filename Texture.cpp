#include "Texture.h"

//public
Texture::Texture(VkFilter filter)
{
	height = 0;
	this->filter = filter;
	width = 0;
}

Texture::~Texture() {}

void Texture::create(LogicalDevice logicalDevice,
    uint32_t height,
    uint32_t width,
    VkFormat format,
    VkImageAspectFlags imageAspect,
    VkImageUsageFlagBits usage,
    VkSampleCountFlagBits sampleCount,
    bool enableAnisotropy,
    VkBorderColor borderColor)
{
    image.create(logicalDevice, height, width, format, usage, sampleCount, 1, 0, false);
    imageView.create(logicalDevice, image.getImage(), VK_IMAGE_VIEW_TYPE_2D, image.getMipLevels(), format, imageAspect, 1);
    sampler.create(logicalDevice, filter, image.getMipLevels(), format, enableAnisotropy, borderColor, VK_COMPARE_OP_ALWAYS);

    this->height = height;
    this->width = width;
}

void Texture::destroy(LogicalDevice logicalDevice)
{
    sampler.destroy(logicalDevice);
    imageView.destroy(logicalDevice);
    image.destroy(logicalDevice);
}

uint32_t Texture::getHeight() { return height; }
Image Texture::getImage() { return image; }
VkImageView Texture::getImageView() { return imageView.getImageView(); }
VkSampler Texture::getSampler() { return sampler.getSampler(); }
uint32_t Texture::getWidth() { return width; }