#pragma once
#include "vulkan/vulkan_core.h"

#include <vector>

class Instance
{
public:
	Instance();
	~Instance();

	void create();
	void destroy();
	VkInstance getInstance();

private:
	VkInstance instance;

	std::vector<const char*> getRequiredExtensions();
};