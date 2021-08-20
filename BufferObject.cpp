#include "BufferObject.h"

//public
BufferObject::BufferObject()
{
	buffer = VK_NULL_HANDLE;
	bufferMemory = VK_NULL_HANDLE;
}

BufferObject::~BufferObject() {}

void BufferObject::destroy(const LogicalDevice& logicalDevice)
{
	vkDestroyBuffer(logicalDevice.getDevice(), buffer, nullptr);
	vkFreeMemory(logicalDevice.getDevice(), bufferMemory, nullptr);
}

VkBuffer BufferObject::getBuffer() { return buffer; }