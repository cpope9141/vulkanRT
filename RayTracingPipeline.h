#pragma once
#include "Pipeline.h"

#include "vulkan/vulkan_core.h"

class RayTracingPipeline : public Pipeline
{
public:
	RayTracingPipeline();
	~RayTracingPipeline();

	void create(LogicalDevice& logicalDevice);
	virtual uint32_t getGroupCount() = 0;

protected:
	virtual std::vector<VkRayTracingShaderGroupCreateInfoKHR> createShaderGroups(LogicalDevice& logicalDevice) = 0;
};