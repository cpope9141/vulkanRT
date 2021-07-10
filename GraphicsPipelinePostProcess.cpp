#include "GraphicsPipelinePostProcess.h"

#include <iostream>

const uint8_t PUSH_CONSTANT_SIZE = 16 * sizeof(float);
const VkShaderStageFlagBits PUSH_CONSTANT_STAGES = VK_SHADER_STAGE_VERTEX_BIT;

//public
GraphicsPipelinePostProcess::GraphicsPipelinePostProcess()
{
    shaderStages = &shaderStagesPostProcess;
}

GraphicsPipelinePostProcess::~GraphicsPipelinePostProcess() {}

uint8_t GraphicsPipelinePostProcess::getPushConstantSize() { return PUSH_CONSTANT_SIZE; }
VkShaderStageFlagBits GraphicsPipelinePostProcess::getPushConstantStages() { return PUSH_CONSTANT_STAGES; }

//protected
void GraphicsPipelinePostProcess::createDescriptorPool(LogicalDevice logicalDevice)
{
    std::vector<VkDescriptorPoolSize> poolSizes;
    //poolSizes.resize(2);
    poolSizes.resize(1);
    VkDescriptorPoolCreateInfo poolInfo = {};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;
    
    //poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //poolSizes[1].descriptorCount = 1;

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

    if (VK_SUCCESS != vkCreateDescriptorPool(logicalDevice.getDevice(), &poolInfo, nullptr, &descriptorPool))
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }
}

void GraphicsPipelinePostProcess::createDescriptorSetLayout(LogicalDevice logicalDevice)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    //bindings.resize(2);
    bindings.resize(1);
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};

    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].pImmutableSamplers = nullptr;
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    //bindings[1].binding = 1;
    //bindings[1].descriptorCount = 1;
    //bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //bindings[1].pImmutableSamplers = nullptr;
    //bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings = bindings.data();

    if (VK_SUCCESS != vkCreateDescriptorSetLayout(logicalDevice.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout))
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }
}

void GraphicsPipelinePostProcess::createPipelineLayout(LogicalDevice logicalDevice)
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