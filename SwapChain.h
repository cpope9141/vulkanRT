#pragma once
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Frame.h"
#include "Framebuffer.h"
#include "Image.h"
#include "ImageView.h"
#include "LogicalDevice.h"
#include "RenderPassPresentation.h"
#include "vulkan/vulkan_core.h"

#include <GLFW/glfw3.h>
#include <map>

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	void create(LogicalDevice logicalDevice, CommandPool commandPool);
	void destroy(LogicalDevice logicalDevice, CommandPool commandPool);
	RenderPassPresentation getRenderPass();

private:
	std::vector<CommandBuffer> commandBuffers;
	Image depthImage;
	ImageView depthImageView;
	std::vector<Framebuffer> framebuffers;
	VkExtent2D imageExtent;
	VkFormat imageFormat;
	std::vector<VkImage> images;
	std::vector<ImageView> imageViews;
	std::map<int, Frame> imagesInFlight;
	std::vector<Frame> inFlightFrames;
	RenderPassPresentation renderPass;
	VkSwapchainKHR swapChain;

	void allocateCommandBuffers(LogicalDevice logicalDevice, CommandPool commandPool);
	VkExtent2D chooseImageExtent(VkSurfaceCapabilitiesKHR capabilities, GLFWwindow* window);
	VkPresentModeKHR choosePresentMode(std::vector<VkPresentModeKHR> availablePresentModes);
	VkSurfaceFormatKHR chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats);
	void createDepthResources(LogicalDevice logicalDevice, CommandPool commandPool);
	void createFramebuffers(LogicalDevice logicalDevice);
	void createImageViews(LogicalDevice logicalDevice);
	void createSyncObjects(LogicalDevice logicalDevice);
	void destroyDepthResources(LogicalDevice logicalDevice);
	void destroyFramebuffers(LogicalDevice logicalDevice);
	void destroyImageViews(LogicalDevice logicalDevice);
	void destroySyncObjects(LogicalDevice logicalDevice);
	void freeCommandBuffers(LogicalDevice logicalDevice);
};