#include "UniformBufferShaderBindingTable.h"

//public
UniformBufferShaderBindingTable::UniformBufferShaderBindingTable() {}
UniformBufferShaderBindingTable::~UniformBufferShaderBindingTable() {}

void UniformBufferShaderBindingTable::update(LogicalDevice& logicalDevice, const void* src)
{
    void* ptr = nullptr;

    vkMapMemory(logicalDevice.getDevice(), bufferMemory, 0, getSize(), 0, &ptr);

    std::memcpy(ptr, src, getSize());

    vkUnmapMemory(logicalDevice.getDevice(), bufferMemory);
}