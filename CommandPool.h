#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class CommandPool
{
public:
	CommandPool();
	~CommandPool();

	void create(LogicalDevice& logicalDevice);
	void destroy(LogicalDevice& logicalDevice);
	VkCommandPool getCommandPool();

private:
	VkCommandPool commandPool;
};