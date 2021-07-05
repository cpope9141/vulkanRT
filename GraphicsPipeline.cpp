#include "GraphicsPipeline.h"
#include "QuadVertexData.h"

#include <iostream>

//public
GraphicsPipeline::GraphicsPipeline() {}
GraphicsPipeline::~GraphicsPipeline() {}

void GraphicsPipeline::create(LogicalDevice logicalDevice, VkRenderPass renderPass)
{
    VkGraphicsPipelineCreateInfo pipelineInfo = {};

    std::vector<VkPipelineShaderStageCreateInfo> stages = shaderStages->shaderStageCreateInfos(logicalDevice);
    VkVertexInputBindingDescription bindingDescription = getBindingDescription();
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = getAttributeDescriptions();
    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineColorBlendAttachmentState colorBlendAttachment = colorBlendAttachmentState();

    VkPipelineVertexInputStateCreateInfo vertexInputState = vertexInputStateCreateInfo(bindingDescription, attributeDescriptions);
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = inputAssemblyStateCreateInfo();
    VkPipelineViewportStateCreateInfo viewportState = viewportStateCreateInfo();
    VkPipelineRasterizationStateCreateInfo rasterizationStage = rasterizationStateCreateInfo();
    VkPipelineMultisampleStateCreateInfo multisamplingState = multisamplingStateCreateInfo(logicalDevice.getPhysicalDevice().getSampleCount());
    VkPipelineDepthStencilStateCreateInfo depthStencil = depthStencilStateCreateInfo();
    VkPipelineColorBlendStateCreateInfo colorBlendState = colorBlendStateCreateInfo(colorBlendAttachment);
    VkPipelineDynamicStateCreateInfo dynamicState = dynamicStateCreateInfo(dynamicStates);

    createDescriptorSetLayout(logicalDevice);
    createPipelineLayout(logicalDevice);

    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stages.size();
    pipelineInfo.pStages = stages.data();
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationStage;
    pipelineInfo.pMultisampleState = &multisamplingState;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.pDynamicState = &dynamicState;

    if (VK_SUCCESS == vkCreateGraphicsPipelines(logicalDevice.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline))
    {
        createDescriptorPool(logicalDevice);
    }
    else
    {
        throw std::runtime_error("Failed to create graphics pipeline");
    }

    shaderStages->destroy(logicalDevice);
}

//protected
VkPipelineColorBlendAttachmentState GraphicsPipeline::colorBlendAttachmentState()
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    return colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo GraphicsPipeline::colorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState& colorBlendAttachment)
{
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_NO_OP;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    return colorBlending;
}

VkPipelineDepthStencilStateCreateInfo GraphicsPipeline::depthStencilStateCreateInfo() {
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.back.compareOp = VK_COMPARE_OP_ALWAYS;

    return depthStencil;
}

VkPipelineDynamicStateCreateInfo GraphicsPipeline::dynamicStateCreateInfo(std::vector<VkDynamicState>& dynamicStates)
{
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};

    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = dynamicStates.size();
    dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

    return dynamicStateCreateInfo;
}

VkPipelineMultisampleStateCreateInfo GraphicsPipeline::multisamplingStateCreateInfo(VkSampleCountFlagBits samples)
{
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.minSampleShading = MIN_SAMPLE_SHADING;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    return multisampling;
}

VkPipelineRasterizationStateCreateInfo GraphicsPipeline::rasterizationStateCreateInfo()
{
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    return rasterizer;
}

VkPipelineVertexInputStateCreateInfo GraphicsPipeline::vertexInputStateCreateInfo(VkVertexInputBindingDescription& bindingDescription,
    std::vector<VkVertexInputAttributeDescription>& attributeDescriptions)
{
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};

    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    return vertexInputStateCreateInfo;
}

//private
VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::inputAssemblyStateCreateInfo()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
    inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    return inputAssemblyStateCreateInfo;
}

VkPipelineViewportStateCreateInfo GraphicsPipeline::viewportStateCreateInfo()
{
    VkExtent2D imageExtent = {};

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = imageExtent.width;
    viewport.height = imageExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent = imageExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    return viewportState;
}