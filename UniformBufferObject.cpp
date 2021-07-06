#include "UniformBufferObject.h"

#include "MemoryUtilities.h"

//public
UniformBufferObject::UniformBufferObject()
{
    size = 0;
}

UniformBufferObject::~UniformBufferObject() {}

void UniformBufferObject::create(LogicalDevice logicalDevice)
{
    create(logicalDevice, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, getSize());
}

void UniformBufferObject::create(LogicalDevice logicalDevice, VkBufferUsageFlagBits usage, VkDeviceSize size)
{
    createBuffer(logicalDevice,
        size,
        usage,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &buffer,
        &bufferMemory);

    this->size = size;
}

VkDeviceSize UniformBufferObject::getSize() { return size; }