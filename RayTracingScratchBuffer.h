#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class RayTracingScratchBuffer
{
public:
	RayTracingScratchBuffer();
	~RayTracingScratchBuffer();

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	VkDeviceAddress deviceAddress;

	void destroy(LogicalDevice& logicalDevice);
};