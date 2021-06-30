#include "Instance.h"
#include "Renderer.h"
#include "ValidationLayer.h"
#include "vulkan/vulkan.h"
#include "WindowManager.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

const char* APPLICATION_NAME = "Vulkan Renderer";
const char* ENGINE_NAME = "No Engine";

//public
Instance::Instance()
{
    instance = VK_NULL_HANDLE;
}

Instance::~Instance() {}

void Instance::create()
{
    if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport())
    {
        throw std::runtime_error("Validation Layer requested but not supported");
    }

    VkApplicationInfo applicationInfo = {};
    VkInstanceCreateInfo instanceCreateInfo = {};
    std::vector<const char*> requiredExtensions;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = APPLICATION_NAME;
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = ENGINE_NAME;
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 2, 0);

    requiredExtensions = getRequiredExtensions();
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());

    if (ENABLE_VALIDATION_LAYERS)
    {
        populateDebugMessengerCreateInfo(debugCreateInfo);
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }

    if (VK_SUCCESS == vkCreateInstance(&instanceCreateInfo, nullptr, &instance))
    {
        createSurface(instance);
        if (ENABLE_VALIDATION_LAYERS) { setupDebugMessenger(instance); }
    }
    else 
    {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
}

void Instance::destroy()
{
    if (ENABLE_VALIDATION_LAYERS) { destroyDebugMessenger(instance); }
    destroySurface(instance);
    vkDestroyInstance(instance, nullptr);
}

VkInstance Instance::getInstance() { return instance; }

//private
std::vector<const char*> Instance::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}