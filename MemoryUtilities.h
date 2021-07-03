#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

int findMemoryType(LogicalDevice logicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);