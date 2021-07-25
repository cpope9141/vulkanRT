#include "ShaderStagesRayTracing.h"

//public
ShaderStagesRayTracing::ShaderStagesRayTracing()
{
    closestHitShaderModule = VK_NULL_HANDLE;
    missShaderModule = VK_NULL_HANDLE;
    raygenShaderModule = VK_NULL_HANDLE;
}

ShaderStagesRayTracing::~ShaderStagesRayTracing() {}

void ShaderStagesRayTracing::destroy(LogicalDevice& logicalDevice)
{
    vkDestroyShaderModule(logicalDevice.getDevice(), closestHitShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice.getDevice(), missShaderModule, nullptr);
    vkDestroyShaderModule(logicalDevice.getDevice(), raygenShaderModule, nullptr);
}