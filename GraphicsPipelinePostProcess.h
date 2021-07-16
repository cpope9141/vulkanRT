#pragma once
#include "GraphicsPipeline.h"
#include "ShaderStagesPostProcess.h"

class GraphicsPipelinePostProcess : public GraphicsPipeline
{
public:
	GraphicsPipelinePostProcess();
	~GraphicsPipelinePostProcess();

	virtual uint8_t getPushConstantSize() override;
	virtual VkShaderStageFlagBits getPushConstantStages() override;

protected:
	virtual void createDescriptorPool(LogicalDevice logicalDevice) override;
	virtual void createDescriptorSetLayout(LogicalDevice logicalDevice) override;
	virtual void createPipelineLayout(LogicalDevice logicalDevice) override;
	virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() override;
	virtual VkVertexInputBindingDescription getBindingDescription() override;

private:
	ShaderStagesPostProcess shaderStagesPostProcess;
};