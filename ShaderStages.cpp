#include "ShaderStages.h"

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