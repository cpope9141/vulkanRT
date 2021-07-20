#pragma once
#include "PhysicalDevice.h"
#include "vulkan/vulkan_core.h"

class LogicalDevice
{
public:
	LogicalDevice();
	~LogicalDevice();

	void create(PhysicalDevice& physicalDevice);
	void destroy();
	VkDevice getDevice();
	VkQueue getGraphicsQueue();
	PhysicalDevice* getPhysicalDevicePtr();
	VkQueue getPresentQueue();

private:
	VkDevice device;
	VkQueue graphicsQueue;
	PhysicalDevice* physicalDevice;
	VkQueue presentQueue;
};