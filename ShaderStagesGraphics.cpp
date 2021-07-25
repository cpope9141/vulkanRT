#include "ShaderStagesGraphics.h"

//public
ShaderStagesGraphics::ShaderStagesGraphics() {}
ShaderStagesGraphics::~ShaderStagesGraphics() {}

void ShaderStagesGraphics::destroy(LogicalDevice& logicalDevice)
{
    vkDestroyShaderModule(logicalDevice.getDevice(), vertexShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice.getDevice(), fragmentShaderModule, nullptr);
}

std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesGraphics::shaderStageCreateInfos(LogicalDevice& logicalDevice)
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.resize(2);
    const char* entryPoint = "main";

    vertexShaderModule = createShaderModule(logicalDevice, readBytecode(getShaderPathVertex()));
    fragmentShaderModule = createShaderModule(logicalDevice, readBytecode(getShaderPathFragment()));

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertexShaderModule;
    shaderStages[0].pName = entryPoint;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragmentShaderModule;
    shaderStages[1].pName = entryPoint;

    return shaderStages;
}