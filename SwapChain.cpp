#include "SwapChain.h"

#include "PhysicalDevice.h"
#include "WindowManager.h"

#include <algorithm>
#include <iostream>

const int MAX_FRAMES_IN_FLIGHT = 2;

//public
SwapChain::SwapChain()
{
    currentFrameIndex = 0;
    imageExtent = { 0, 0 };
    imageFormat = VK_FORMAT_UNDEFINED;
    swapChain = VK_NULL_HANDLE;
    nextImageIndex = 0;
}

SwapChain::~SwapChain() {}

VkResult SwapChain::acquireNextImage(LogicalDevice logicalDevice)
{
    Frame currentFrame = inFlightFrames[currentFrameIndex];
    VkResult result = VK_SUCCESS;
    VkFence fence = currentFrame.getFence();

    vkWaitForFences(logicalDevice.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

    result = vkAcquireNextImageKHR(logicalDevice.getDevice(),
        swapChain,
        UINT64_MAX,
        currentFrame.getImageAvailableSemaphore(),
        VK_NULL_HANDLE,
        &nextImageIndex);

    if (VK_SUCCESS != result)
    {
        throw std::runtime_error("Failed to acquire next image from swap chain");
    }

    return result;
}

void SwapChain::create(LogicalDevice logicalDevice, CommandPool commandPool)
{
	VkSurfaceKHR surface = getSurface();
	PhysicalDevice physicalDevice = logicalDevice.getPhysicalDevice();
	SwapChainSupportInfo swapChainSupportInfo = physicalDevice.querySwapChainSupport(physicalDevice.getPhysicalDevice(), surface);
	VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapChainSupportInfo.formats);
    VkPresentModeKHR presentMode = choosePresentMode(swapChainSupportInfo.presentModes);
    VkExtent2D extent = chooseImageExtent(swapChainSupportInfo.capabilities, getWindow());
    uint32_t imageCount = swapChainSupportInfo.capabilities.minImageCount + 1;
    VkSwapchainCreateInfoKHR createInfo = {};
    QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = { indices.getGraphicsFamily().value(), indices.getPresentFamily().value() };

    if (swapChainSupportInfo.capabilities.maxImageCount > 0 && imageCount > swapChainSupportInfo.capabilities.maxImageCount)
    {
        imageCount = swapChainSupportInfo.capabilities.maxImageCount;
    }

    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (indices.getGraphicsFamily().value() != indices.getPresentFamily().value())
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupportInfo.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (VK_SUCCESS == vkCreateSwapchainKHR(logicalDevice.getDevice(), &createInfo, nullptr, &swapChain))
    {
        vkGetSwapchainImagesKHR(logicalDevice.getDevice(), swapChain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice.getDevice(), swapChain, &imageCount, images.data());

        imageFormat = surfaceFormat.format;
        imageExtent = extent;

        createImageViews(logicalDevice);
        renderPass.create(logicalDevice, imageFormat);
        createDepthResources(logicalDevice, commandPool);
        createFramebuffers(logicalDevice);
        allocateCommandBuffers(logicalDevice, commandPool);
        createSyncObjects(logicalDevice);
    }
    else
    {
        throw std::runtime_error("Failed to create swap chain");
    }
}

void SwapChain::destroy(LogicalDevice logicalDevice, CommandPool commandPool)
{
    destroySyncObjects(logicalDevice);
    freeCommandBuffers(logicalDevice);
    destroyFramebuffers(logicalDevice);
    destroyDepthResources(logicalDevice);
    renderPass.destroy(logicalDevice);
    destroyImageViews(logicalDevice);
    vkDestroySwapchainKHR(logicalDevice.getDevice(), swapChain, nullptr);
}

std::vector<CommandBuffer> SwapChain::getCommandBuffers() { return commandBuffers; }
std::vector<Framebuffer> SwapChain::getFramebuffers() { return framebuffers; }
VkExtent2D SwapChain::getImageExtent() { return imageExtent; }
uint32_t SwapChain::getNextImageIndex() { return nextImageIndex; }
RenderPassPresentation SwapChain::getRenderPass() { return renderPass; }

VkResult SwapChain::present(LogicalDevice logicalDevice)
{
    VkResult result = VK_ERROR_DEVICE_LOST;
    Frame currentFrame = inFlightFrames[currentFrameIndex];
    VkPresentInfoKHR presentInfo = {};
    VkSemaphore renderFinishedSemaphore = currentFrame.getRenderFinishedSemaphore();

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &nextImageIndex;

    result = vkQueuePresentKHR(logicalDevice.getPresentQueue(), &presentInfo);

    if (VK_SUCCESS == result || VK_ERROR_OUT_OF_DATE_KHR == result || VK_SUBOPTIMAL_KHR == result)
    {
        currentFrameIndex = (currentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    return result;
}

VkResult SwapChain::submitDrawCommand(LogicalDevice logicalDevice, VkCommandBuffer commandBuffer)
{
    Frame currentFrame = inFlightFrames[currentFrameIndex];
    VkSubmitInfo submitInfo = {};
    VkSemaphore imageAvailableSemaphore = currentFrame.getImageAvailableSemaphore();
    VkSemaphore renderFinishedSemaphore = currentFrame.getRenderFinishedSemaphore();
    VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkFence currentFrameFence = currentFrame.getFence();

    if (imagesInFlight.find(nextImageIndex) != imagesInFlight.end())
    {
        VkFence fence = imagesInFlight[nextImageIndex].getFence();
        vkWaitForFences(logicalDevice.getDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
    }

    imagesInFlight[nextImageIndex] = currentFrame;

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = &waitDstStageMask;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphore;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkResetFences(logicalDevice.getDevice(), 1, &currentFrameFence);

    return vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &submitInfo, currentFrameFence);
}

//private
void SwapChain::allocateCommandBuffers(LogicalDevice logicalDevice, CommandPool commandPool)
{
    for (Framebuffer framebuffer : framebuffers)
    {
        CommandBuffer commandBuffer(commandPool);
        commandBuffer.allocate(logicalDevice);
        commandBuffers.push_back(commandBuffer);
    }
}

VkExtent2D SwapChain::chooseImageExtent(VkSurfaceCapabilitiesKHR capabilities, GLFWwindow* window) {
    VkExtent2D actualExtent = capabilities.currentExtent;

    if (UINT32_MAX == capabilities.currentExtent.width)
    {
        int height, width;

        glfwGetFramebufferSize(window, &width, &height);
        
        actualExtent.height = height;
        actualExtent.width = width;

        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    }

    return actualExtent;
}

VkPresentModeKHR SwapChain::choosePresentMode(std::vector<VkPresentModeKHR> availablePresentModes)
{
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

    for (VkPresentModeKHR availableMode : availablePresentModes)
    {
        if (VK_PRESENT_MODE_MAILBOX_KHR == availableMode)
        {
            presentMode = availableMode;
            break;
        }
    }

    return presentMode;
}

VkSurfaceFormatKHR SwapChain::chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats)
{
    VkSurfaceFormatKHR chosenFormat = availableFormats[0];

    for (VkSurfaceFormatKHR availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            chosenFormat = availableFormat;
            break;
        }
    }

    return chosenFormat;
}

void SwapChain::createDepthResources(LogicalDevice logicalDevice, CommandPool commandPool)
{
    VkFormat depthFormat = logicalDevice.getPhysicalDevice().getDepthFormat();

    depthImage.create(logicalDevice,
        imageExtent.height,
        imageExtent.width,
        depthFormat,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_SAMPLE_COUNT_1_BIT,
        1,
        0,
        false);

    depthImageView.create(logicalDevice, depthImage.getImage(), VK_IMAGE_VIEW_TYPE_2D, 1, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}

void SwapChain::createFramebuffers(LogicalDevice logicalDevice)
{
    for (ImageView imageView : imageViews)
    {
        Framebuffer framebuffer;
        framebuffer.create(logicalDevice, renderPass.getRenderPass(), imageExtent.height, imageExtent.width, imageView.getImageView(), depthImageView.getImageView(), VK_NULL_HANDLE);
        framebuffers.push_back(framebuffer);
    }
}

void SwapChain::createImageViews(LogicalDevice logicalDevice)
{
    for (VkImage image : images)
    {
        ImageView imageView;
        imageView.create(logicalDevice, image, VK_IMAGE_VIEW_TYPE_2D, 1, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        imageViews.push_back(imageView);
    }
}

void SwapChain::createSyncObjects(LogicalDevice logicalDevice)
{
    inFlightFrames.resize(MAX_FRAMES_IN_FLIGHT);

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        Frame frame;
        frame.create(logicalDevice);
        inFlightFrames[i] = frame;
    }
}

void SwapChain::destroyDepthResources(LogicalDevice logicalDevice)
{
    depthImageView.destroy(logicalDevice);
    depthImage.destroy(logicalDevice);
}

void SwapChain::destroyFramebuffers(LogicalDevice logicalDevice)
{
    for (Framebuffer framebuffer : framebuffers) { framebuffer.destroy(logicalDevice); }
    framebuffers.clear();
}

void SwapChain::destroyImageViews(LogicalDevice logicalDevice)
{
    for (ImageView imageView : imageViews) { imageView.destroy(logicalDevice); }
    imageViews.clear();
}

void SwapChain::destroySyncObjects(LogicalDevice logicalDevice)
{
    for (Frame frame : inFlightFrames) { frame.destroy(logicalDevice); }
    inFlightFrames.clear();
    imagesInFlight.clear();
}

void SwapChain::freeCommandBuffers(LogicalDevice logicalDevice)
{
    for (CommandBuffer commandBuffer : commandBuffers) { commandBuffer.freeCommandBuffer(logicalDevice); }
    commandBuffers.clear();
}