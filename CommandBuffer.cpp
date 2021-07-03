#include "CommandBuffer.h"

#include <iostream>

CommandBuffer::CommandBuffer(CommandPool commandPool)
{
	commandBuffer = VK_NULL_HANDLE;
	this->commandPool = commandPool;
}

CommandBuffer::~CommandBuffer() {}

void CommandBuffer::allocate(LogicalDevice logicalDevice)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    VkDevice device = logicalDevice.getDevice();

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.getCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (VK_SUCCESS != vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer)) {
        throw std::runtime_error("Fialed to allocate command buffer");
    }
}

void CommandBuffer::beginOneTimeCommandBuffer(LogicalDevice logicalDevice)
{
    VkCommandBufferBeginInfo beginInfo = {};

    allocate(logicalDevice);

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (VK_SUCCESS != vkBeginCommandBuffer(commandBuffer, &beginInfo)) {
        throw std::runtime_error("Fialed to begin one time command buffer");
    }
}

void CommandBuffer::freeCommandBuffer(LogicalDevice logicalDevice)
{
    vkFreeCommandBuffers(logicalDevice.getDevice(), commandPool.getCommandPool(), 1, &commandBuffer);
}

VkCommandBuffer CommandBuffer::getCommandBuffer() { return commandBuffer; }

void CommandBuffer::submitOneTimeCommandBuffer(LogicalDevice logicalDevice)
{
    VkSubmitInfo submitInfo = {};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    if (VK_SUCCESS == vkEndCommandBuffer(commandBuffer) &&
        VK_SUCCESS == vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) &&
        VK_SUCCESS == vkQueueWaitIdle(logicalDevice.getGraphicsQueue())) {
        freeCommandBuffer(logicalDevice);
    }
    else {
        throw std::runtime_error("Fialed to submit one time command buffer");
    }
}