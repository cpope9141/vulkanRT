#include "LogicalDevice.h"

#include "Renderer.h"
#include "ValidationLayer.h"

#include <iostream>

//public
LogicalDevice::LogicalDevice()
{
	device = VK_NULL_HANDLE;
    graphicsQueue = VK_NULL_HANDLE;
    presentQueue = VK_NULL_HANDLE;
}

LogicalDevice::~LogicalDevice() {}

void LogicalDevice::create(PhysicalDevice physicalDevice)
{
    const float priority = 1.0f;
    QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();
    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
    VkDeviceCreateInfo deviceCreateInfo = {};
    std::set<uint32_t> uniqueQueueFamilies = indices.uniqueQueueFamlies();
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfo.queueCount = 1;
        deviceQueueCreateInfos.push_back(queueCreateInfo);
    }

    physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
    physicalDeviceFeatures.sampleRateShading = VK_TRUE;

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (ENABLE_VALIDATION_LAYERS)
    {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }

    if (VK_SUCCESS == vkCreateDevice(physicalDevice.getPhysicalDevice(), &deviceCreateInfo, nullptr, &device))
    {
        vkGetDeviceQueue(device, indices.getGraphicsFamily().value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.getPresentFamily().value(), 0, &presentQueue);

        this->physicalDevice = physicalDevice;
    }
    else
    {
        throw std::runtime_error("Failed to create logical device");
    }
}

void LogicalDevice::destroy() { vkDestroyDevice(device, nullptr); }
VkDevice LogicalDevice::getDevice() { return device; }
VkQueue LogicalDevice::getGraphicsQueue() { return graphicsQueue; }
PhysicalDevice LogicalDevice::getPhysicalDevice() { return physicalDevice; }
VkQueue LogicalDevice::getPresentQueue() { return presentQueue; }
