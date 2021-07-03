#pragma once
#include "CommandPool.h"
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class CommandBuffer
{
public:
	CommandBuffer(CommandPool commandPool);
	~CommandBuffer();

	void allocate(LogicalDevice logicalDevice);
	void beginOneTimeCommandBuffer(LogicalDevice logicalDevice);
	void freeCommandBuffer(LogicalDevice logicalDevice);
	VkCommandBuffer getCommandBuffer();
	void submitOneTimeCommandBuffer(LogicalDevice logicalDevice);

private:
	VkCommandBuffer commandBuffer;
	CommandPool commandPool;
};