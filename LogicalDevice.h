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
	VkDevice getDevice() const;
	VkQueue getGraphicsQueue() const;
	const PhysicalDevice* getPhysicalDevicePtr() const;
	VkQueue getPresentQueue();

private:
	VkDevice device;
	VkQueue graphicsQueue;
	const PhysicalDevice* physicalDevice;
	VkQueue presentQueue;
};