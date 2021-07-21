#include "Pipeline.h"

//public
Pipeline::Pipeline()
{
	descriptorPool = VK_NULL_HANDLE;
	descriptorSetLayout = VK_NULL_HANDLE;
	pipeline = VK_NULL_HANDLE;
	pipelineLayout = VK_NULL_HANDLE;
	shaderStages = nullptr;
}

Pipeline::~Pipeline() {}

void Pipeline::destroy(LogicalDevice& logicalDevice)
{
	vkDestroyPipeline(logicalDevice.getDevice(), pipeline, nullptr);
    vkDestroyDescriptorPool(logicalDevice.getDevice(), descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(logicalDevice.getDevice(), descriptorSetLayout, nullptr);
    vkDestroyPipelineLayout(logicalDevice.getDevice(), pipelineLayout, nullptr);
}

VkDescriptorPool Pipeline::getDescriptorPool() { return descriptorPool; }
VkDescriptorSetLayout Pipeline::getDescriptorSetLayout() { return descriptorSetLayout; }
VkPipeline Pipeline::getPipeline() { return pipeline; }
VkPipelineLayout Pipeline::getPipelineLayout() { return pipelineLayout; }