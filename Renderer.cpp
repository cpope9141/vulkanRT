#include "Renderer.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::create()
{
	instance.create();
	physicalDevice.selectPhysicalDevice(instance.getInstance());
}

void Renderer::destroy()
{
	instance.destroy();
}