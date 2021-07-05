#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

#include <vector>

class ShaderStages
{
public:
	ShaderStages();
	~ShaderStages();

	virtual void destroy(LogicalDevice logicalDevice) = 0;
	virtual std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(LogicalDevice logicalDevice) = 0;

protected:
	VkShaderModule createShaderModule(LogicalDevice logicalDevice, const std::vector<char>& spirvCode);
};