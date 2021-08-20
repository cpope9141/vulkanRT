#pragma once
#include "CommandPool.h"
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class CommandBuffer
{
public:
	CommandBuffer(CommandPool commandPool);
	~CommandBuffer();

	void allocate(const LogicalDevice& logicalDevice);
	void beginOneTimeCommandBuffer(const LogicalDevice& logicalDevice);
	void freeCommandBuffer(const LogicalDevice& logicalDevice);
	VkCommandBuffer getCommandBuffer();
	void submitOneTimeCommandBuffer(const LogicalDevice& logicalDevice);

private:
	VkCommandBuffer commandBuffer;
	CommandPool commandPool;
};