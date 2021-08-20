#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class Sampler
{
public:
	Sampler();
	~Sampler();

	void create(const LogicalDevice& logicalDevice, VkFilter filter, uint32_t mipLevels, VkFormat format, bool anisotropyEnable, VkBorderColor borderColor, VkCompareOp compareOp);
	void destroy(const LogicalDevice& logicalDevice);
	VkSampler getSampler();

private:
	VkSampler sampler;

	bool formatIsFilterable(VkPhysicalDevice physicalDevice, VkFormat format, VkImageTiling tiling);
};