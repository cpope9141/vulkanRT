#pragma once
#include "CommandPool.h"
#include "Instance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

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

private:
	CommandPool commandPool;
	Instance instance;
	LogicalDevice logicalDevice;
	PhysicalDevice physicalDevice;
};