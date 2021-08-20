#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

void createBuffer(const LogicalDevice& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* pBuffer, VkDeviceMemory* pBufferMemory);
uint32_t findMemoryType(const LogicalDevice& logicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);