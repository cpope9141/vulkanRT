#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class CommandPool
{
public:
	CommandPool();
	~CommandPool();

	void create(const LogicalDevice& logicalDevice);
	void destroy(const LogicalDevice& logicalDevice);
	VkCommandPool getCommandPool();

private:
	VkCommandPool commandPool;
};