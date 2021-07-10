#pragma once
#include "CommandPool.h"
#include "DescriptorSetPostProcess.h"
#include "GraphicsPipelinePostProcess.h"
#include "Instance.h"
#include "LogicalDevice.h"
#include "Panel.h"
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
	void prepare();
	void submit();
	void waitForDeviceIdle();

private:
	CommandPool commandPool;
	DescriptorSetPostProcess descriptorSetPostProcess;
	GraphicsPipelinePostProcess graphicsPipelinePostProcess;
	Instance instance;
	LogicalDevice logicalDevice;
	Panel panel;
	PhysicalDevice physicalDevice;
	SwapChain swapChain;
	UniformBufferProjection uboOrthoProjection = UniformBufferProjection(TYPE_ORTHOGRAPHIC);

	void addDrawCommands(VkDescriptorSet ds, Pipeline* gp, void* pc, uint32_t pcSize, VertexData* vertexData);
	void createSwapChainObjects();
	void destroySwapChainObjects();
	void drawPostProcess();
	void recreateSwapChain();
};