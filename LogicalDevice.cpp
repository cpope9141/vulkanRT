#include "LogicalDevice.h"

#include "Renderer.h"
#include "ValidationLayer.h"

#include <iostream>

//public
LogicalDevice::LogicalDevice()
{
	device = VK_NULL_HANDLE;
    graphicsQueue = VK_NULL_HANDLE;
    physicalDevice = nullptr;
    presentQueue = VK_NULL_HANDLE;
}

LogicalDevice::~LogicalDevice() {}

void LogicalDevice::create(PhysicalDevice& physicalDevice)
{
    const float priority = 1.0f;
    QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();
    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
    VkDeviceCreateInfo deviceCreateInfo = {};
    std::set<uint32_t> uniqueQueueFamilies = indices.uniqueQueueFamlies();
    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

    VkPhysicalDeviceBufferDeviceAddressFeatures enabledBufferDeviceAddressFeatures = {};
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR enabledRayTracingPipelineFeatures = {};
    VkPhysicalDeviceAccelerationStructureFeaturesKHR enabledAccelerationStructureFeatures = {};
    VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures = {};

    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2 = {};

    enabledBufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
    enabledBufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;

    enabledRayTracingPipelineFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
    enabledRayTracingPipelineFeatures.rayTracingPipeline = VK_TRUE;
    enabledRayTracingPipelineFeatures.pNext = &enabledBufferDeviceAddressFeatures;

    enabledAccelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    enabledAccelerationStructureFeatures.accelerationStructure = VK_TRUE;
    enabledAccelerationStructureFeatures.pNext = &enabledRayTracingPipelineFeatures;

    descriptorIndexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
    descriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
    descriptorIndexingFeatures.pNext = &enabledAccelerationStructureFeatures;

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
    physicalDeviceFeatures.shaderInt64 = VK_TRUE;

    physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physicalDeviceFeatures2.features = physicalDeviceFeatures;
    physicalDeviceFeatures2.pNext = &descriptorIndexingFeatures;
    deviceCreateInfo.pEnabledFeatures = nullptr;
    deviceCreateInfo.pNext = &physicalDeviceFeatures2;

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
    //deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
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

        this->physicalDevice = &physicalDevice;
    }
    else
    {
        throw std::runtime_error("Failed to create logical device");
    }
}

void LogicalDevice::destroy() { vkDestroyDevice(device, nullptr); }
VkDevice LogicalDevice::getDevice() { return device; }
VkQueue LogicalDevice::getGraphicsQueue() { return graphicsQueue; }
PhysicalDevice* LogicalDevice::getPhysicalDevicePtr() { return physicalDevice; }
VkQueue LogicalDevice::getPresentQueue() { return presentQueue; }
