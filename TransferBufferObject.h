#pragma once
#include "BufferObject.h"

#include "CommandPool.h"
#include "vulkan/vulkan_core.h"

class TransferBufferObject : public BufferObject
{
public:
	TransferBufferObject(VkBufferUsageFlags usage);
	~TransferBufferObject();

	void create(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const void* src, VkDeviceSize bufferSize);

private:
	VkBufferUsageFlags usage;

	void copyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};