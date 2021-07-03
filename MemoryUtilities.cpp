#include "MemoryUtilities.h"

#include "LogicalDevice.h"

#include <iostream>

int findMemoryType(LogicalDevice logicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    int memoryType = -1;
    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(logicalDevice.getPhysicalDevice().getPhysicalDevice(), &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if (0 != (typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            memoryType = i;
            break;
        }
    }

    if (0 > memoryType) { throw std::runtime_error("Failed to find suitable memory type"); }

    return memoryType;
}