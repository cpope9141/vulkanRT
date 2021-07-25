#include "ShaderStages.h"

#include <fstream>
#include <ios>
#include <iostream>

//public
ShaderStages::ShaderStages() {}
ShaderStages::~ShaderStages() {}

//protected
VkShaderModule ShaderStages::createShaderModule(LogicalDevice logicalDevice, const std::vector<char>& spirvCode)
{
    VkShaderModuleCreateInfo createInfo = {};
    VkShaderModule shaderModule = VK_NULL_HANDLE;

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirvCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvCode.data());

    if (VK_SUCCESS != vkCreateShaderModule(logicalDevice.getDevice(), &createInfo, nullptr, &shaderModule))
    {
        throw std::runtime_error("Failed to create shader module");
    }

    return shaderModule;
}

std::vector<char> ShaderStages::readBytecode(const std::string& relativePath)
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