#pragma once
#include "PhysicalDevice.h"
#include "vulkan/vulkan_core.h"

class LogicalDevice
{
public:
	LogicalDevice();
	~LogicalDevice();

	void create(PhysicalDevice physicalDevice);
	void destroy();
	VkDevice getDevice();
	VkQueue getGraphicsQueue();
	PhysicalDevice getPhysicalDevice();

private:
	VkDevice device;
	VkQueue graphicsQueue;
	PhysicalDevice physicalDevice;
	VkQueue presentQueue;
};