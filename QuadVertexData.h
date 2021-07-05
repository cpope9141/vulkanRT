#pragma once
#include "vulkan/vulkan_core.h"

#include <vector>

VkVertexInputBindingDescription getBindingDescription();
std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();