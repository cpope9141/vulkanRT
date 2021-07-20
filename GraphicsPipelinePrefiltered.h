#pragma once
#include "GraphicsPipeline.h"

#include "ShaderStagesPrefiltered.h"
#include "vulkan/vulkan_core.h"

#include <glm/glm.hpp>

struct PrefilteredPushConstant
{
	alignas(16) glm::mat4 viewProjectionMatrix;
	float roughness;
	uint32_t sampleCount;
};

class GraphicsPipelinePrefiltered : public GraphicsPipeline
{
public:
	GraphicsPipelinePrefiltered();
	~GraphicsPipelinePrefiltered();

	virtual uint8_t getPushConstantSize() override;
	virtual VkShaderStageFlags getPushConstantStages() override;

protected:
	virtual void createDescriptorPool(LogicalDevice logicalDevice) override;
	virtual void createDescriptorSetLayout(LogicalDevice logicalDevice) override;
	virtual void createPipelineLayout(LogicalDevice logicalDevice) override;
	virtual VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo() override;
	virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() override;
	virtual VkVertexInputBindingDescription getBindingDescription() override;
	virtual VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo(VkSampleCountFlagBits samples) override;
	virtual VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo() override;

private:
	ShaderStagesPrefiltered shaderStagesPrefiltered;
};