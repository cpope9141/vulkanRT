#include "Renderer.h"

//public
Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::create()
{
	instance.create();
	physicalDevice.selectPhysicalDevice(instance.getInstance());
	logicalDevice.create(physicalDevice);
	commandPool.create(logicalDevice);
	createSwapChainObjects();
	graphicsPipelinePostProcess.create(logicalDevice, swapChain.getRenderPass().getRenderPass());
}

void Renderer::destroy()
{
	graphicsPipelinePostProcess.destroy(logicalDevice);
	destroySwapChainObjects();
	commandPool.destroy(logicalDevice);
	logicalDevice.destroy();
	instance.destroy();
}

LogicalDevice Renderer::getLogicalDevice() { return logicalDevice; }

//private
void Renderer::createSwapChainObjects()
{
	swapChain.create(logicalDevice, commandPool);
}

void Renderer::destroySwapChainObjects()
{
	swapChain.destroy(logicalDevice, commandPool);
}