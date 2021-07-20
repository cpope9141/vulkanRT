#pragma once
#include "GraphicsPipeline.h"

#include "ShaderStagesIntegratedBRDF.h"

#include <glm/glm.hpp>

struct IntegratedBRDFPushConstant
{
	alignas(16) glm::mat4 modelViewMatrix;
};

class GraphicsPipelineIntegratedBRDF : public GraphicsPipeline
{
public:
	GraphicsPipelineIntegratedBRDF();
	~GraphicsPipelineIntegratedBRDF();

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
	ShaderStagesIntegratedBRDF shaderStagesIntegratedBRDF;
};