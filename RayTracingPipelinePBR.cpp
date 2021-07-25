#include "RayTracingPipelinePBR.h"

#include <iostream>

const uint32_t GROUP_COUNT = 4;
const uint8_t PUSH_CONSTANT_SIZE = 0;
const VkShaderStageFlags PUSH_CONSTANT_STAGES = 0;

//public
RayTracingPipelinePBR::RayTracingPipelinePBR()
{
    shaderStages = &shaderStagesRTPBR;
}

RayTracingPipelinePBR::~RayTracingPipelinePBR() {}

uint32_t RayTracingPipelinePBR::getGroupCount() { return GROUP_COUNT; }
uint8_t RayTracingPipelinePBR::getPushConstantSize() { return PUSH_CONSTANT_SIZE; }
VkShaderStageFlags RayTracingPipelinePBR::getPushConstantStages() { return PUSH_CONSTANT_STAGES; }

//protected
void RayTracingPipelinePBR::createDescriptorPool(LogicalDevice& logicalDevice)
{
    std::vector<VkDescriptorPoolSize> poolSizes;
    poolSizes.resize(5);
    VkDescriptorPoolCreateInfo poolInfo = {};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    poolSizes[0].descriptorCount = 1;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[1].descriptorCount = 1;

    poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[2].descriptorCount = 1;

    poolSizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[3].descriptorCount = 1;

    poolSizes[4].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[4].descriptorCount = (1 + 3 + 1);

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

    if (VK_SUCCESS != vkCreateDescriptorPool(logicalDevice.getDevice(), &poolInfo, nullptr, &descriptorPool))
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }
}

void RayTracingPipelinePBR::createDescriptorSetLayout(LogicalDevice& logicalDevice)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(9);
    VkDescriptorSetLayoutCreateInfo descriptorSetlayoutCI = {};

    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = (VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR);

    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR;

    bindings[2].binding = 2;
    bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[2].descriptorCount = 1;
    bindings[2].stageFlags = (VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR);

    bindings[3].binding = 3;
    bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[3].descriptorCount = 1;
    bindings[3].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

    bindings[4].binding = 4;
    bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[4].descriptorCount = 5;//needs to know the number of BLAS * 5
    bindings[4].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

    bindings[5].binding = 5;
    bindings[5].descriptorCount = 1;
    bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[5].pImmutableSamplers = nullptr;
    bindings[5].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

    bindings[6].binding = 6;
    bindings[6].descriptorCount = 1;
    bindings[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[6].pImmutableSamplers = nullptr;
    bindings[6].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

    bindings[7].binding = 7;
    bindings[7].descriptorCount = 1;
    bindings[7].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[7].pImmutableSamplers = nullptr;
    bindings[7].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

    bindings[8].binding = 8;
    bindings[8].descriptorCount = 1;
    bindings[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[8].pImmutableSamplers = nullptr;
    bindings[8].stageFlags = VK_SHADER_STAGE_MISS_BIT_KHR;

    descriptorSetlayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetlayoutCI.bindingCount = static_cast<uint32_t>(bindings.size());
    descriptorSetlayoutCI.pBindings = bindings.data();

    if (VK_SUCCESS != vkCreateDescriptorSetLayout(logicalDevice.getDevice(), &descriptorSetlayoutCI, nullptr, &descriptorSetLayout))
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }
}

void RayTracingPipelinePBR::createPipelineLayout(LogicalDevice& logicalDevice)
{
    VkPipelineLayoutCreateInfo pipelineLayoutCI = {};

    pipelineLayoutCI.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCI.setLayoutCount = 1;
    pipelineLayoutCI.pSetLayouts = &descriptorSetLayout;

    if (VK_SUCCESS != vkCreatePipelineLayout(logicalDevice.getDevice(), &pipelineLayoutCI, nullptr, &pipelineLayout))
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

std::vector<VkRayTracingShaderGroupCreateInfoKHR> RayTracingPipelinePBR::createShaderGroups(LogicalDevice& logicalDevice)
{
    std::vector<VkRayTracingShaderGroupCreateInfoKHR> shaderGroups;
    shaderGroups.resize(GROUP_COUNT);

    shaderGroups[0].sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
    shaderGroups[0].type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
    shaderGroups[0].generalShader = 0;
    shaderGroups[0].closestHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[0].anyHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[0].intersectionShader = VK_SHADER_UNUSED_KHR;

    shaderGroups[1].sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
    shaderGroups[1].type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
    shaderGroups[1].generalShader = 1;
    shaderGroups[1].closestHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[1].anyHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[1].intersectionShader = VK_SHADER_UNUSED_KHR;

    shaderGroups[2].sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
    shaderGroups[2].type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
    shaderGroups[2].generalShader = 2;
    shaderGroups[2].closestHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[2].anyHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[2].intersectionShader = VK_SHADER_UNUSED_KHR;

    shaderGroups[3].sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
    shaderGroups[3].type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
    shaderGroups[3].generalShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[3].closestHitShader = 3;
    shaderGroups[3].anyHitShader = VK_SHADER_UNUSED_KHR;
    shaderGroups[3].intersectionShader = VK_SHADER_UNUSED_KHR;

    return shaderGroups;
}