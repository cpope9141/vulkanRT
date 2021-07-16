#pragma once
#include "CerberusRT.h"
#include "CommandPool.h"
#include "DescriptorSetPBR.h"
#include "DescriptorSetPostProcess.h"
#include "FramebufferObjectMultiSample.h"
#include "GraphicsPipelinePBR.h"
#include "GraphicsPipelinePostProcess.h"
#include "Instance.h"
#include "LogicalDevice.h"
#include "Panel.h"
#include "PhysicalDevice.h"
#include "RenderPassMultiSample.h"
#include "SwapChain.h"
#include "UniformBufferPBRLighting.h"
#include "UniformBufferProjection.h"
#include "UniformBufferStaticModelPBR.h"

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
	CerberusRT cerberusRT;
	CommandPool commandPool;
	DescriptorSetPBR descriptorSetPBR;
	std::vector<DescriptorSetPostProcess> descriptorSetPostProcess;
	std::vector<FramebufferObjectMultiSample> fbosMultiSample;
	GraphicsPipelinePBR graphicsPipelinePBR;
	GraphicsPipelinePostProcess graphicsPipelinePostProcess;
	Instance instance;
	LogicalDevice logicalDevice;
	Panel panel;
	PhysicalDevice physicalDevice;
	RenderPassMultiSample renderPassMultiSample;
	SwapChain swapChain;
	UniformBufferPBRLighting uboLighting;
	UniformBufferProjection uboOrthographic = UniformBufferProjection(TYPE_ORTHOGRAPHIC);
	UniformBufferProjection uboPerspective = UniformBufferProjection(TYPE_PERSPECTIVE);
	UniformBufferStaticModelPBR uboStaticModel;
	glm::mat4 viewMatrix;

	void addDrawCommands(VkDescriptorSet ds, GraphicsPipeline* gp, void* pc, uint32_t pcSize, VertexData* vertexData);
	void createSwapChainObjects();
	void destroySwapChainObjects();
	void drawPostProcess();
	void drawStaticModelPBR(ModelRT staticModelPBR);
	void recreateSwapChain();
};