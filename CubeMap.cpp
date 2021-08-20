#include "CubeMap.h"

static const uint8_t LAYER_COUNT = 6;

//public
CubeMap::CubeMap(VkFilter filter) : Texture(filter) {}
CubeMap::~CubeMap() {}

void CubeMap::create(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const HDRResource resources[6])
{
    image.create(logicalDevice, commandPool, resources);
    imageView.create(logicalDevice,
        image.getImage(),
        VK_IMAGE_VIEW_TYPE_CUBE,
        image.getMipLevels(),
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_IMAGE_ASPECT_COLOR_BIT,
        6);
    sampler.create(logicalDevice,
        filter,
        image.getMipLevels(),
        VK_FORMAT_R32G32B32A32_SFLOAT,
        true,
        VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        VK_COMPARE_OP_NEVER);

    this->height = resources[0].height;
    this->width = resources[0].width;
}

void CubeMap::create(const LogicalDevice& logicalDevice, uint32_t height, uint32_t width, VkFormat format, bool genMipLevels)
{
    image.create(logicalDevice,
        height,
        width,
        format,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_SAMPLE_COUNT_1_BIT,
        LAYER_COUNT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        genMipLevels);
    imageView.create(logicalDevice,
        image.getImage(),
        VK_IMAGE_VIEW_TYPE_CUBE,
        image.getMipLevels(),
        format,
        VK_IMAGE_ASPECT_COLOR_BIT,
        LAYER_COUNT);
    sampler.create(logicalDevice,
        filter,
        image.getMipLevels(),
        format,
        false,
        VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        VK_COMPARE_OP_NEVER);

    this->height = height;
    this->width = width;
}