#pragma once
#include "CommandPool.h"
#include "GraphicsPipelinePostProcess.h"
#include "Instance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "UniformBufferProjection.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>

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
	void prepare();
	void submit();
	void waitForDeviceIdle();

private:
	CommandPool commandPool;
	GraphicsPipelinePostProcess graphicsPipelinePostProcess;
	Instance instance;
	LogicalDevice logicalDevice;
	PhysicalDevice physicalDevice;
	SwapChain swapChain;
	UniformBufferProjection uboOrthoProjection = UniformBufferProjection(TYPE_ORTHOGRAPHIC);
	glm::mat4 viewMatrix;

	void createSwapChainObjects();
	void destroySwapChainObjects();
	void recreateSwapChain();
};