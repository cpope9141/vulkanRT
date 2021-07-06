#include "Renderer.h"

#include "WindowManager.h"

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

	uboOrthoProjection.create(logicalDevice);
	uboOrthoProjection.update(logicalDevice, HEIGHT, WIDTH, 1.0f, 96.0f);
}

void Renderer::destroy()
{
	uboOrthoProjection.destroy(logicalDevice);

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