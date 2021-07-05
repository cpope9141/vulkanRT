#pragma once
#include "QueueFamilyIndices.h"
#include "vulkan/vulkan_core.h"

#include <vector>

struct SwapChainSupportInfo
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class PhysicalDevice
{
public:
	PhysicalDevice();
	~PhysicalDevice();

	VkFormat getDepthFormat();
	VkPhysicalDevice getPhysicalDevice();
	QueueFamilyIndices getQueueFamilyIndices();
	VkSampleCountFlagBits getSampleCount();
	SwapChainSupportInfo querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void selectPhysicalDevice(VkInstance instance);

private:
	VkFormat depthFormat;
	VkPhysicalDevice physicalDevice;
	QueueFamilyIndices queueFamilyIndices;
	VkSampleCountFlagBits sampleCount;

	bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
	VkFormat findDepthFormat();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	VkSampleCountFlagBits findSampleCount();
	VkFormat findSupportedFormat(std::vector<VkFormat> formatCandidates, VkImageTiling tiling, VkFormatFeatureFlagBits features);
	bool physicalDeviceMeetsRequirements(VkPhysicalDevice physicalDevice);
};