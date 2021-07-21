#pragma once
#include "GraphicsPipeline.h"

#include "ShaderStagesIrradiance.h"
#include "vulkan/vulkan_core.h"

#include <glm/glm.hpp>

struct IrradiancePushConstant
{
	alignas(16) glm::mat4 viewProjectionMatrix;
	float deltaPhi;
	float deltaTheta;
};

class GraphicsPipelineIrradiance : public GraphicsPipeline
{
public:
	GraphicsPipelineIrradiance();
	~GraphicsPipelineIrradiance();

	virtual uint8_t getPushConstantSize() override;
	virtual VkShaderStageFlags getPushConstantStages() override;

protected:
	virtual void createDescriptorPool(LogicalDevice& logicalDevice) override;
	virtual void createDescriptorSetLayout(LogicalDevice& logicalDevice) override;
	virtual void createPipelineLayout(LogicalDevice& logicalDevice) override;
	virtual VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo() override;
	virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() override;
	virtual VkVertexInputBindingDescription getBindingDescription() override;
	virtual VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo(VkSampleCountFlagBits samples) override;
	virtual VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo() override;

private:
	ShaderStagesIrradiance shaderStagesIrradiance;
};