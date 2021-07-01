#pragma once
#include "vulkan/vulkan_core.h"

#include <vector>

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

bool checkValidationLayerSupport();
void destroyDebugMessenger(VkInstance instance);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
void setupDebugMessenger(VkInstance instance);