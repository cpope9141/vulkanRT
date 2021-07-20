#include "CommandPool.h"

#include "QueueFamilyIndices.h"

#include <iostream>

CommandPool::CommandPool()
{
    commandPool = VK_NULL_HANDLE;
}

CommandPool::~CommandPool() {}

void CommandPool::create(LogicalDevice logicalDevice)
{
    QueueFamilyIndices queueFamilyIndices = logicalDevice.getPhysicalDevicePtr()->getQueueFamilyIndices();
    VkCommandPoolCreateInfo commandPoolCreateInfo = {};

    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.getGraphicsFamily().value();

    if (VK_SUCCESS != vkCreateCommandPool(logicalDevice.getDevice(), &commandPoolCreateInfo, nullptr, &commandPool))
    {
        throw std::runtime_error("Failed to create command pool");
    }
}

void CommandPool::destroy(LogicalDevice logicalDevice) { vkDestroyCommandPool(logicalDevice.getDevice(), commandPool, nullptr); }
VkCommandPool CommandPool::getCommandPool() { return commandPool; }