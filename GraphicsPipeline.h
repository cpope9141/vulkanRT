#pragma once
#include "LogicalDevice.h"
#include "Pipeline.h"
#include "vulkan/vulkan_core.h"

const float MIN_SAMPLE_SHADING = 0.2f;

class GraphicsPipeline : public Pipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline();

	void create(LogicalDevice logicalDevice, VkRenderPass renderPass);

protected:
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState();
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState& colorBlendAttachment);
	virtual VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo();
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStates);
	virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() = 0;
	virtual VkVertexInputBindingDescription getBindingDescription() = 0;
	virtual VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo(VkSampleCountFlagBits samples);
	virtual VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo();
	virtual VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(VkVertexInputBindingDescription& bindingDescription, std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);

private:
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo();
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo();
};