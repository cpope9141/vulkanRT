#pragma once
#include "GraphicsPipeline.h"

#include "ShaderStagesPBR.h"

class GraphicsPipelinePBR : public GraphicsPipeline
{
public:
	GraphicsPipelinePBR();
	~GraphicsPipelinePBR();

	virtual uint8_t getPushConstantSize() override;
	virtual VkShaderStageFlags getPushConstantStages() override;

protected:
	virtual void createDescriptorPool(LogicalDevice& logicalDevice) override;
	virtual void createDescriptorSetLayout(LogicalDevice& logicalDevice) override;
	virtual void createPipelineLayout(LogicalDevice& logicalDevice) override;
	virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() override;
	virtual VkVertexInputBindingDescription getBindingDescription() override;
	virtual VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo(VkSampleCountFlagBits samples) override;

private:
	ShaderStagesPBR shaderStagesPBR;
};