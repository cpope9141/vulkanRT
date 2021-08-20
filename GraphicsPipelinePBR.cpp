#include "GraphicsPipelinePBR.h"

#include "StaticMeshVertexData.h"

#include <iostream>

const uint8_t PUSH_CONSTANT_SIZE = 3 * sizeof(float);
const VkShaderStageFlags PUSH_CONSTANT_STAGES = VK_SHADER_STAGE_FRAGMENT_BIT;

//public
GraphicsPipelinePBR::GraphicsPipelinePBR()
{
    shaderStages = &shaderStagesPBR;
}

GraphicsPipelinePBR::~GraphicsPipelinePBR() {}

uint8_t GraphicsPipelinePBR::getPushConstantSize() { return PUSH_CONSTANT_SIZE; }
VkShaderStageFlags GraphicsPipelinePBR::getPushConstantStages() { return PUSH_CONSTANT_STAGES; }

//protected
void GraphicsPipelinePBR::createDescriptorPool(LogicalDevice& logicalDevice)
{
    std::vector<VkDescriptorPoolSize> poolSizes;
    poolSizes.resize(2);
    VkDescriptorPoolCreateInfo poolInfo = {};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 3;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 8;

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 3;

    if (VK_SUCCESS != vkCreateDescriptorPool(logicalDevice.getDevice(), &poolInfo, nullptr, &descriptorPool))
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }
}

void GraphicsPipelinePBR::createDescriptorSetLayout(LogicalDevice& logicalDevice)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(11);
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};

    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].pImmutableSamplers = nullptr;
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    bindings[1].binding = 1;
    bindings[1].descriptorCount = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[1].pImmutableSamplers = nullptr;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[2].binding = 2;
    bindings[2].descriptorCount = 1;
    bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[2].pImmutableSamplers = nullptr;
    bindings[2].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    bindings[3].binding = 3;
    bindings[3].descriptorCount = 1;
    bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[3].pImmutableSamplers = nullptr;
    bindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[4].binding = 4;
    bindings[4].descriptorCount = 1;
    bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[4].pImmutableSamplers = nullptr;
    bindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[5].binding = 5;
    bindings[5].descriptorCount = 1;
    bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[5].pImmutableSamplers = nullptr;
    bindings[5].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[6].binding = 6;
    bindings[6].descriptorCount = 1;
    bindings[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[6].pImmutableSamplers = nullptr;
    bindings[6].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[7].binding = 7;
    bindings[7].descriptorCount = 1;
    bindings[7].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[7].pImmutableSamplers = nullptr;
    bindings[7].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    bindings[8].binding = 8;
    bindings[8].descriptorCount = 1;
    bindings[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[8].pImmutableSamplers = nullptr;
    bindings[8].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    bindings[9].binding = 9;
    bindings[9].descriptorCount = 1;
    bindings[9].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[9].pImmutableSamplers = nullptr;
    bindings[9].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    bindings[10].binding = 10;
    bindings[10].descriptorCount = 1;
    bindings[10].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[10].pImmutableSamplers = nullptr;
    bindings[10].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (VK_SUCCESS != vkCreateDescriptorSetLayout(logicalDevice.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout))
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }
}

void GraphicsPipelinePBR::createPipelineLayout(LogicalDevice& logicalDevice)
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

std::vector<VkVertexInputAttributeDescription> GraphicsPipelinePBR::getAttributeDescriptions() { return StaticMeshVertexData::getAttributeDescriptions(); }
VkVertexInputBindingDescription GraphicsPipelinePBR::getBindingDescription() { return StaticMeshVertexData::getBindingDescription(); }

VkPipelineMultisampleStateCreateInfo GraphicsPipelinePBR::multisamplingStateCreateInfo(VkSampleCountFlagBits samples)
{
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.minSampleShading = MIN_SAMPLE_SHADING;
    multisampling.rasterizationSamples = samples;

    return multisampling;
}