#include "RayTracingScratchBuffer.h"

//public
RayTracingScratchBuffer::RayTracingScratchBuffer()
{
    buffer = VK_NULL_HANDLE;
    bufferMemory = VK_NULL_HANDLE;
    deviceAddress = 0;
}

RayTracingScratchBuffer::~RayTracingScratchBuffer() {}

void RayTracingScratchBuffer::destroy(LogicalDevice& logicalDevice)
{
    vkDestroyBuffer(logicalDevice.getDevice(), buffer, nullptr);
    vkFreeMemory(logicalDevice.getDevice(), bufferMemory, nullptr);
}