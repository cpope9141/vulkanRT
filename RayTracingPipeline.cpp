#include "RayTracingPipeline.h"

#include <iostream>

//public
RayTracingPipeline::RayTracingPipeline() {}
RayTracingPipeline::~RayTracingPipeline() {}

void RayTracingPipeline::create(LogicalDevice& logicalDevice)
{
	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR =
		reinterpret_cast<PFN_vkCreateRayTracingPipelinesKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkCreateRayTracingPipelinesKHR"));
	VkRayTracingPipelineCreateInfoKHR rayTracingPipelineCI = {};

	std::vector<VkPipelineShaderStageCreateInfo> stages = shaderStages->shaderStageCreateInfos(logicalDevice);
	createDescriptorSetLayout(logicalDevice);
	createPipelineLayout(logicalDevice);
	std::vector<VkRayTracingShaderGroupCreateInfoKHR> groups = createShaderGroups(logicalDevice);

	rayTracingPipelineCI.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
	rayTracingPipelineCI.stageCount = static_cast<uint32_t>(stages.size());
	rayTracingPipelineCI.pStages = stages.data();
	rayTracingPipelineCI.groupCount = static_cast<uint32_t>(groups.size());
	rayTracingPipelineCI.pGroups = groups.data();
	rayTracingPipelineCI.maxPipelineRayRecursionDepth = 2;
	rayTracingPipelineCI.layout = pipelineLayout;

	if (VK_SUCCESS == vkCreateRayTracingPipelinesKHR(logicalDevice.getDevice(), VK_NULL_HANDLE, VK_NULL_HANDLE, 1, &rayTracingPipelineCI, nullptr, &pipeline))
	{
		createDescriptorPool(logicalDevice);
	}
	else
	{
		throw std::runtime_error("Failed to create ray tracing pipeline");
	}

	shaderStages->destroy(logicalDevice);
}