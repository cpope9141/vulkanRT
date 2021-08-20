#pragma once
#include "LogicalDevice.h"
#include "ShaderStages.h"
#include "vulkan/vulkan_core.h"

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	void destroy(const LogicalDevice& logicalDevice);
	VkDescriptorPool getDescriptorPool();
	VkDescriptorSetLayout getDescriptorSetLayout();
	VkPipeline getPipeline();
	VkPipelineLayout getPipelineLayout();
	virtual uint8_t getPushConstantSize() = 0;
	virtual VkShaderStageFlags getPushConstantStages() = 0;

protected:
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	ShaderStages* shaderStages;

	virtual void createDescriptorPool(LogicalDevice& logicalDevice) = 0;
	virtual void createDescriptorSetLayout(LogicalDevice& logicalDevice) = 0;
	virtual void createPipelineLayout(LogicalDevice& logicalDevice) = 0;
};