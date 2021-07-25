#pragma once
#include "BufferObject.h"

#include "CommandPool.h"
#include "vulkan/vulkan_core.h"

class TransferBufferObject : public BufferObject
{
public:
	TransferBufferObject(VkBufferUsageFlags usage);
	~TransferBufferObject();

	void create(LogicalDevice& logicalDevice, CommandPool& commandPool, const void* src, VkDeviceSize bufferSize);

private:
	VkBufferUsageFlags usage;

	void copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};