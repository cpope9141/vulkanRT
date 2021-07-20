#include "GraphicsPipelineIntegratedBRDF.h"

#include "QuadVertexData.h"

#include <iostream>

const uint8_t PUSH_CONSTANT_SIZE = sizeof(IntegratedBRDFPushConstant);
const VkShaderStageFlags PUSH_CONSTANT_STAGES = VK_SHADER_STAGE_VERTEX_BIT;

//public
GraphicsPipelineIntegratedBRDF::GraphicsPipelineIntegratedBRDF()
{
    shaderStages = &shaderStagesIntegratedBRDF;
}

GraphicsPipelineIntegratedBRDF::~GraphicsPipelineIntegratedBRDF() {}

uint8_t GraphicsPipelineIntegratedBRDF::getPushConstantSize() { return PUSH_CONSTANT_SIZE; }
VkShaderStageFlags GraphicsPipelineIntegratedBRDF::getPushConstantStages() { return PUSH_CONSTANT_STAGES; }

//protected
void GraphicsPipelineIntegratedBRDF::createDescriptorPool(LogicalDevice logicalDevice)
{
    std::vector<VkDescriptorPoolSize> poolSizes;
    poolSizes.resize(1);
    VkDescriptorPoolCreateInfo poolInfo = {};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

    if (VK_SUCCESS != vkCreateDescriptorPool(logicalDevice.getDevice(), &poolInfo, nullptr, &descriptorPool))
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }
}

void GraphicsPipelineIntegratedBRDF::createDescriptorSetLayout(LogicalDevice logicalDevice)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(1);
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};

    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].pImmutableSamplers = nullptr;
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    if (VK_SUCCESS != vkCreateDescriptorSetLayout(logicalDevice.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout))
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }
}

void GraphicsPipelineIntegratedBRDF::createPipelineLayout(LogicalDevice logicalDevice)
{
    VkPushConstantRange pushConstRange = {};
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};

    pushConstRange.stageFlags = PUSH_CONSTANT_STAGES;
    pushConstRange.offset = 0;
    pushConstRange.size = PUSH_CONSTANT_SIZE;

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstRange;

    if (VK_SUCCESS != vkCreatePipelineLayout(logicalDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout))
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

std::vector<VkVertexInputAttributeDescription> GraphicsPipelineIntegratedBRDF::getAttributeDescriptions() { return QuadVertexData::getAttributeDescriptions(); }
VkVertexInputBindingDescription GraphicsPipelineIntegratedBRDF::getBindingDescription() { return QuadVertexData::getBindingDescription(); }

VkPipelineDepthStencilStateCreateInfo GraphicsPipelineIntegratedBRDF::depthStencilStateCreateInfo()
{
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_FALSE;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencil.back.compareOp = VK_COMPARE_OP_ALWAYS;

    return depthStencil;
}

VkPipelineMultisampleStateCreateInfo GraphicsPipelineIntegratedBRDF::multisamplingStateCreateInfo(VkSampleCountFlagBits samples)
{
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    return multisampling;
}

VkPipelineRasterizationStateCreateInfo GraphicsPipelineIntegratedBRDF::rasterizationStateCreateInfo()
{
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    return rasterizer;
}