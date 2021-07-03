#include "PhysicalDevice.h"

#include "Renderer.h"
#include "WindowManager.h"

#include <iostream>

//public
PhysicalDevice::PhysicalDevice()
{
    depthFormat = VK_FORMAT_UNDEFINED;
    physicalDevice = VK_NULL_HANDLE;
    sampleCount = VK_SAMPLE_COUNT_1_BIT;
}

PhysicalDevice::~PhysicalDevice() {}

VkFormat PhysicalDevice::getDepthFormat() { return depthFormat; }
QueueFamilyIndices PhysicalDevice::getQueueFamilyIndices() { return queueFamilyIndices; }
VkPhysicalDevice PhysicalDevice::getPhysicalDevice() { return physicalDevice; }

void PhysicalDevice::selectPhysicalDevice(VkInstance instance)
{
    uint32_t deviceCount = 0;
    std::vector<VkPhysicalDevice> physicalDevices;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (0 < deviceCount)
    {
        physicalDevices.resize(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        for (VkPhysicalDevice physicalDevice : physicalDevices)
        {
            if (physicalDeviceMeetsRequirements(physicalDevice))
            {
                this->physicalDevice = physicalDevice;
                sampleCount = findSampleCount();
                depthFormat = findDepthFormat();
                break;
            }
        }

        if (VK_NULL_HANDLE == physicalDevice) { throw std::runtime_error("Failed to find a suitable GPU"); }
    }
    else
    {
        throw std::runtime_error("No GPU with Vulkan support found");
    }
}

SwapChainSupportInfo PhysicalDevice::querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    SwapChainSupportInfo supportInfo;
    uint32_t count = 0;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &supportInfo.capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);

    if (0 != count)
    {
        supportInfo.formats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, supportInfo.formats.data());
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);

    if (0 != count)
    {
        supportInfo.presentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, supportInfo.presentModes.data());
    }

    return supportInfo;
}

//private
bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    bool result = true;
    uint32_t extensionCount = 0;
    std::vector<VkExtensionProperties> availableExtensions;

    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    availableExtensions.resize(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    for (int i = 0; i < deviceExtensions.size(); i++) {
        bool found = false;

        for (int j = 0; j < availableExtensions.size(); j++) {
            if (0 == strcmp(deviceExtensions[i], availableExtensions[j].extensionName)) {
                found = true;
                break;
            }
        }

        if (!found) {
            result = false;
            break;
        }
    }

    return result;
}

VkFormat PhysicalDevice::findSupportedFormat(std::vector<VkFormat> formats, VkImageTiling tiling, VkFormatFeatureFlagBits features)
{
    VkFormatProperties props = {};

    for (VkFormat format : formats)
    {
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported format");
}

VkFormat PhysicalDevice::findDepthFormat()
{
    return findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    VkBool32 presentSupport = VK_FALSE;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    for (int i = 0; i < queueFamilyProperties.size(); i++)
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (0 != (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) { indices.setGraphicsFamily(i); }
        if (VK_TRUE == presentSupport) { indices.setPresentFamily(i); }
        if (indices.requiredFamiliesFound()) { break; }
    }

    return indices;
}

bool PhysicalDevice::physicalDeviceMeetsRequirements(VkPhysicalDevice physicalDevice)
{
    bool result = false;
    VkSurfaceKHR surface = getSurface();
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

    if (checkDeviceExtensionSupport(physicalDevice) && queueFamilyIndices.requiredFamiliesFound())
    {
        VkPhysicalDeviceProperties physicalDeviceProperties = {};
        SwapChainSupportInfo supportInfo = querySwapChainSupport(physicalDevice, surface);

        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

        result = (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU == physicalDeviceProperties.deviceType) &&
            (!supportInfo.formats.empty() && !supportInfo.presentModes.empty());
    }

    if (result)
    {
        this->queueFamilyIndices = queueFamilyIndices;
    }

    return result;
}

VkSampleCountFlagBits PhysicalDevice::findSampleCount()
{
    VkPhysicalDeviceProperties physicalDeviceProperties = {};
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
    VkSampleCountFlags sampleCountFlags = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    VkSampleCountFlagBits result = VK_SAMPLE_COUNT_1_BIT;

    if ((sampleCountFlags & VK_SAMPLE_COUNT_8_BIT) != 0)
    {
        result = VK_SAMPLE_COUNT_8_BIT;
    }
    else if ((sampleCountFlags & VK_SAMPLE_COUNT_4_BIT) != 0)
    {
        result = VK_SAMPLE_COUNT_4_BIT;
    }
    else if ((sampleCountFlags & VK_SAMPLE_COUNT_2_BIT) != 0)
    {
        result = VK_SAMPLE_COUNT_2_BIT;
    }

    return result;
}