#pragma once
#include "LogicalDevice.h"
#include "vulkan/vulkan_core.h"

class Frame
{
public:
	Frame();
	~Frame();

    void create(const LogicalDevice& logicalDevice);
    void destroy(const LogicalDevice& logicalDevice);
    VkFence getFence();
    VkSemaphore getImageAvailableSemaphore();
    VkSemaphore getRenderFinishedSemaphore();

private:
    VkFence fence;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
};