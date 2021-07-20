#pragma once
#include "BufferObject.h"

#include "LogicalDevice.h"

class UniformBufferObject : public BufferObject
{
public:
	UniformBufferObject();
	~UniformBufferObject();

	void create(LogicalDevice& logicalDevice);
	void create(LogicalDevice& logicalDevice, VkBufferUsageFlagBits usage, VkDeviceSize size);
	VkDeviceSize getSize();

protected:
	VkDeviceSize size;
};