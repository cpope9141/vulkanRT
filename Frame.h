#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class Frame
{
public:
	Frame();
	~Frame();

    void create(LogicalDevice logicalDevice);
    void destroy(LogicalDevice logicalDevice);
    VkFence getFence();
    VkSemaphore getImageAvailableSemaphore();
    VkSemaphore getRenderFinishedSemaphore();

private:
    VkFence fence;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
};