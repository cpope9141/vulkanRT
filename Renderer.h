#pragma once
#include "CommandPool.h"
#include "GraphicsPipelinePostProcess.h"
#include "Instance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "UniformBufferProjection.h"

#define ENABLE_VALIDATION_LAYERS true

const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Renderer
{
public:
	Renderer();
	~Renderer();

	void create();
	void destroy();
	LogicalDevice getLogicalDevice();

private:
	CommandPool commandPool;
	GraphicsPipelinePostProcess graphicsPipelinePostProcess;
	Instance instance;
	LogicalDevice logicalDevice;
	PhysicalDevice physicalDevice;
	SwapChain swapChain;
	UniformBufferProjection uboOrthoProjection{ TYPE_ORTHOGRAPHIC };

	void createSwapChainObjects();
	void destroySwapChainObjects();
};