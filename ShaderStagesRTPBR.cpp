#include "ShaderStagesRTPBR.h"

//public
ShaderStagesRTPBR::ShaderStagesRTPBR()
{
    missShadowShaderModule = VK_NULL_HANDLE;
}

ShaderStagesRTPBR::~ShaderStagesRTPBR() {}

void ShaderStagesRTPBR::destroy(LogicalDevice& logicalDevice)
{
    vkDestroyShaderModule(logicalDevice.getDevice(), missShadowShaderModule, nullptr);

    ShaderStagesRayTracing::destroy(logicalDevice);
}

std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesRTPBR::shaderStageCreateInfos(LogicalDevice& logicalDevice)
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.resize(4);
    const char* entryPoint = "main";

    raygenShaderModule = createShaderModule(logicalDevice, readBytecode(getShaderPathRaygen()));
    missShaderModule = createShaderModule(logicalDevice, readBytecode(getShaderPathMiss()));
    missShadowShaderModule = createShaderModule(logicalDevice, readBytecode(SHADER_PATH_MISS_SHADOW));
    closestHitShaderModule = createShaderModule(logicalDevice, readBytecode(getShaderPathClosestHit()));

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
    shaderStages[0].module = raygenShaderModule;
    shaderStages[0].pName = entryPoint;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_MISS_BIT_KHR;
    shaderStages[1].module = missShaderModule;
    shaderStages[1].pName = entryPoint;

    shaderStages[2].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[2].stage = VK_SHADER_STAGE_MISS_BIT_KHR;
    shaderStages[2].module = missShadowShaderModule;
    shaderStages[2].pName = entryPoint;

    shaderStages[3].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[3].stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    shaderStages[3].module = closestHitShaderModule;
    shaderStages[3].pName = entryPoint;

    return shaderStages;
}

//protected
const char* ShaderStagesRTPBR::getShaderPathClosestHit() { return SHADER_PATH_CLOSEST_HIT; }
const char* ShaderStagesRTPBR::getShaderPathMiss() { return SHADER_PATH_MISS; }
const char* ShaderStagesRTPBR::getShaderPathRaygen() { return SHADER_PATH_RAYGEN; }