#pragma once
#include "UniformBufferObject.h"

class UniformBufferInstance : public UniformBufferObject
{
public:
	UniformBufferInstance();
	~UniformBufferInstance();

	void update(LogicalDevice& logicalDevice, std::vector<VkAccelerationStructureInstanceKHR>& instances);
};