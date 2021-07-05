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
	VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo();
	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStates);
	VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo(VkSampleCountFlagBits samples);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo();
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo(VkVertexInputBindingDescription& bindingDescription, std::vector<VkVertexInputAttributeDescription>& attributeDescriptions);

private:
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo();
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo();
};