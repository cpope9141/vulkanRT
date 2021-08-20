#pragma once
#include "BufferObject.h"

#include "LogicalDevice.h"

class UniformBufferObject : public BufferObject
{
public:
	UniformBufferObject();
	~UniformBufferObject();

	void create(const LogicalDevice& logicalDevice);
	void create(const LogicalDevice& logicalDevice, VkBufferUsageFlags usage, VkDeviceSize size);
	VkDeviceSize getSize();

protected:
	VkDeviceSize size;
};