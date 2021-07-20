#include "MemoryUtilities.h"

#include "LogicalDevice.h"

#include <iostream>

void createBuffer(LogicalDevice logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory)
{
    VkBufferCreateInfo bufferCreateInfo = {};

    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (VK_SUCCESS == vkCreateBuffer(logicalDevice.getDevice(), &bufferCreateInfo, nullptr, pBuffer)) {
        VkMemoryRequirements memoryRequirements = {};
        VkMemoryAllocateFlagsInfo allocFlagsInfo = {};
        VkMemoryAllocateInfo memoryAllocateInfo = {};

        vkGetBufferMemoryRequirements(logicalDevice.getDevice(), *pBuffer, &memoryRequirements);

        if (0 != (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT))
        {
            allocFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
            allocFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
            memoryAllocateInfo.pNext = &allocFlagsInfo;
        }

        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = findMemoryType(logicalDevice, memoryRequirements.memoryTypeBits, properties);

        if (VK_SUCCESS == vkAllocateMemory(logicalDevice.getDevice(), &memoryAllocateInfo, nullptr, pBufferMemory))
        {
            if (VK_SUCCESS != vkBindBufferMemory(logicalDevice.getDevice(), *pBuffer, *pBufferMemory, 0))
            {
                throw std::runtime_error("Failed to bind buffer memory");
            }
        }
        else
        {
            throw std::runtime_error("Failed to allocate buffer memory");
        }
    }
    else
    {
        throw std::runtime_error("Failed to create buffer");
    }
}

uint32_t findMemoryType(LogicalDevice logicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(logicalDevice.getPhysicalDevicePtr()->getPhysicalDevice(), &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if (0 != (typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type");
}