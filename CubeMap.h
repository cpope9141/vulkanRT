#pragma once
#include "Texture.h"

#include "LogicalDevice.h"
#include "Types.h"
#include "vulkan/vulkan_core.h"

class CubeMap : public Texture
{
public:
	CubeMap(VkFilter filter);
	~CubeMap();

	void create(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const HDRResource resources[6]);
	void create(const LogicalDevice& logicalDevice, uint32_t height, uint32_t width, VkFormat format, bool genMipLevels);
};