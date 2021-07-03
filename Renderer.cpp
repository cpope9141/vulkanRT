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
}

void Renderer::destroy()
{
	destroySwapChainObjects();
	commandPool.destroy(logicalDevice);
	logicalDevice.destroy();
	instance.destroy();
}

//private
void Renderer::createSwapChainObjects()
{
	swapChain.create(logicalDevice, commandPool);
}

void Renderer::destroySwapChainObjects()
{
	swapChain.destroy(logicalDevice, commandPool);
}