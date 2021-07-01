#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::create()
{
	instance.create();
	physicalDevice.selectPhysicalDevice(instance.getInstance());
	logicalDevice.create(physicalDevice);
	commandPool.create(logicalDevice);
}

void Renderer::destroy()
{
	commandPool.destroy(logicalDevice);
	logicalDevice.destroy();
	instance.destroy();
}