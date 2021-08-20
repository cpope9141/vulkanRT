#include "TransferBufferObject.h"

#include "CommandBuffer.h"
#include "MemoryUtilities.h"

//public
TransferBufferObject::TransferBufferObject(VkBufferUsageFlags usage)
{
    this->usage = usage;
}

TransferBufferObject::~TransferBufferObject() {}

void TransferBufferObject::create(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const void* src, VkDeviceSize bufferSize)
{
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    void* ptr = nullptr;

    createBuffer(logicalDevice,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingBuffer,
        &stagingBufferMemory);

    vkMapMemory(logicalDevice.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &ptr);
    std::memcpy(ptr, src, bufferSize);
    vkUnmapMemory(logicalDevice.getDevice(), stagingBufferMemory);

    createBuffer(logicalDevice,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,
        VK_MEMORY_HEAP_DEVICE_LOCAL_BIT,
        &buffer,
        &bufferMemory);

    copyBuffer(logicalDevice, commandPool, stagingBuffer, buffer, bufferSize);

    vkDestroyBuffer(logicalDevice.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice.getDevice(), stagingBufferMemory, nullptr);
}

//private
void TransferBufferObject::copyBuffer(const LogicalDevice& logicalDevice, const CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    CommandBuffer commandBuffer(commandPool);
    VkBufferCopy copyRegion = {};

    commandBuffer.beginOneTimeCommandBuffer(logicalDevice);
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer.getCommandBuffer(), srcBuffer, dstBuffer, 1, &copyRegion);
    commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
}