#pragma once
#include "QueueFamilyIndices.h"
#include "vulkan/vulkan_core.h"

#include <vector>

struct SwapChainSupportInfo {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class PhysicalDevice
{
public:
	PhysicalDevice();
	~PhysicalDevice();

	SwapChainSupportInfo querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void selectPhysicalDevice(VkInstance instance);

private:
	VkPhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;

	bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	bool physicalDeviceMeetsRequirements(VkPhysicalDevice physicalDevice);
};