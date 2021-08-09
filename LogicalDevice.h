#pragma once
#include "PhysicalDevice.h"
#include "vulkan/vulkan_core.h"

class LogicalDevice
{
public:
	LogicalDevice();
	~LogicalDevice();

	void create(const PhysicalDevice& physicalDevice);
	void destroy();
	VkDevice getDevice();
	VkQueue getGraphicsQueue();
	const PhysicalDevice* getPhysicalDevicePtr();
	VkQueue getPresentQueue();

private:
	VkDevice device;
	VkQueue graphicsQueue;
	const PhysicalDevice* physicalDevice;
	VkQueue presentQueue;
};