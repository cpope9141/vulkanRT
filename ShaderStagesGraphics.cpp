#include "ShaderStagesGraphics.h"

#include <fstream>
#include <ios>
#include <string>

static std::vector<char> readBytecode(const std::string& relativePath);

//public
ShaderStagesGraphics::ShaderStagesGraphics() {}
ShaderStagesGraphics::~ShaderStagesGraphics() {}

void ShaderStagesGraphics::destroy(LogicalDevice logicalDevice)
{
    vkDestroyShaderModule(logicalDevice.getDevice(), vertexShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice.getDevice(), fragmentShaderModule, nullptr);
}

std::vector<VkPipelineShaderStageCreateInfo> ShaderStagesGraphics::shaderStageCreateInfos(LogicalDevice logicalDevice)
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

static std::vector<char> readBytecode(const std::string& relativePath)
{
    std::ifstream file(relativePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) { throw std::runtime_error("failed to open file!"); }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}