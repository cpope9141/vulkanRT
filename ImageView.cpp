#include "ImageView.h"

#include <iostream>

//public
ImageView::ImageView()
{
    imageView = VK_NULL_HANDLE;
}

ImageView::~ImageView() {}

void ImageView::create(LogicalDevice logicalDevice, VkImage image, VkImageViewType viewType, uint32_t mipLevels, VkFormat format, VkImageAspectFlags imageAspect, uint32_t layerCount)
{
    VkImageViewCreateInfo viewCreateInfo = {};

    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.image = image;
    viewCreateInfo.viewType = viewType;
    viewCreateInfo.format = format;

    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    viewCreateInfo.subresourceRange.aspectMask = imageAspect;
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = mipLevels;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = layerCount;

    if (VK_SUCCESS != vkCreateImageView(logicalDevice.getDevice(), &viewCreateInfo, nullptr, &imageView))
    {
        throw std::runtime_error("Failed to create texture image view");
    }
}

void ImageView::destroy(const LogicalDevice& logicalDevice) { vkDestroyImageView(logicalDevice.getDevice(), imageView, nullptr); }
VkImageView ImageView::getImageView() { return imageView; }