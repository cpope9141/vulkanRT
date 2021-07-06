#include "Sampler.h"

#include <iostream>

//public
Sampler::Sampler()
{
	sampler = VK_NULL_HANDLE;
}

Sampler::~Sampler() {}

void Sampler::create(LogicalDevice logicalDevice, VkFilter filter, uint32_t mipLevels, VkFormat format, bool anisotropyEnable, VkBorderColor borderColor, VkCompareOp compareOp)
{
    VkSamplerCreateInfo samplerInfo = {};

    if (VK_FILTER_NEAREST != filter) {
        filter = formatIsFilterable(logicalDevice.getPhysicalDevice().getPhysicalDevice(), format, VK_IMAGE_TILING_OPTIMAL) ? filter : VK_FILTER_NEAREST;
    }

    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = filter;
    samplerInfo.minFilter = filter;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = anisotropyEnable;
    samplerInfo.maxAnisotropy = anisotropyEnable ? 16.0f : 1.0f;
    samplerInfo.borderColor = borderColor;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = compareOp;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0;
    samplerInfo.maxLod = (float)mipLevels;
    samplerInfo.mipLodBias = 0;

    if (VK_SUCCESS != vkCreateSampler(logicalDevice.getDevice(), &samplerInfo, nullptr, &sampler))
    {
        throw std::runtime_error("Failed to create texture sampler");
    }
}

void Sampler::destroy(LogicalDevice logicalDevice) { vkDestroySampler(logicalDevice.getDevice(), sampler, nullptr); }
VkSampler Sampler::getSampler() { return sampler; }

//private
bool Sampler::formatIsFilterable(VkPhysicalDevice physicalDevice, VkFormat format, VkImageTiling tiling) 
{
    VkFormatProperties formatProps = {};
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
    bool result = false;

    if (VK_IMAGE_TILING_OPTIMAL == tiling)
    {
        result = 0 != (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
    }
    else if (VK_IMAGE_TILING_LINEAR == tiling)
    {
        result = 0 != (formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
    }

    return result;
}