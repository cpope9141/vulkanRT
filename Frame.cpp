#include "Frame.h"

#include <iostream>

//public
Frame::Frame()
{
    fence = VK_NULL_HANDLE;
    imageAvailableSemaphore = VK_NULL_HANDLE;
    renderFinishedSemaphore = VK_NULL_HANDLE;
}

Frame::~Frame() {}

void Frame::create(const LogicalDevice& logicalDevice)
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    VkFenceCreateInfo fenceInfo = {};

    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (VK_SUCCESS != vkCreateSemaphore(logicalDevice.getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) ||
        VK_SUCCESS != vkCreateSemaphore(logicalDevice.getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) ||
        VK_SUCCESS != vkCreateFence(logicalDevice.getDevice(), &fenceInfo, nullptr, &fence))
    {
        throw std::runtime_error("Failed to create synchronization objects");
    }
}

void Frame::destroy(const LogicalDevice& logicalDevice)
{
    vkDestroySemaphore(logicalDevice.getDevice(), renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(logicalDevice.getDevice(), imageAvailableSemaphore, nullptr);
    vkDestroyFence(logicalDevice.getDevice(), fence, nullptr);
}

VkFence Frame::getFence() { return fence; }
VkSemaphore Frame::getImageAvailableSemaphore() { return imageAvailableSemaphore; }
VkSemaphore Frame::getRenderFinishedSemaphore() { return renderFinishedSemaphore; }