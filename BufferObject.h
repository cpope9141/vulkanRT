#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class BufferObject
{
public:
	BufferObject();
	~BufferObject();

	void destroy(LogicalDevice logicalDevice);
	VkBuffer getBuffer();

protected:
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
};