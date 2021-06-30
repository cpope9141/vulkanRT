#include "Renderer.h"
#include "ValidationLayer.h"
#include "vulkan/vk_layer.h"

#include <vector>
#include <iostream>

VkDebugUtilsMessengerEXT debugMessenger;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT <= messageSeverity) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

bool checkValidationLayerSupport()
{
    bool result = true;
    uint32_t layerCount = 0;
    std::vector<VkLayerProperties> availableLayers;

    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    availableLayers.resize(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool found = false;

        for (const auto& layerProperties : availableLayers) {
            if (0 == strcmp(layerName, layerProperties.layerName)) {
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

void destroyDebugMessenger(VkInstance instance)
{
    if (enableValidationLayers) {
        destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void setupDebugMessenger(VkInstance instance)
{
    if (enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        populateDebugMessengerCreateInfo(createInfo);

        if (VK_SUCCESS != createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger)) {
            throw std::runtime_error("Failed to set up debug messenger");
        }
    }
}