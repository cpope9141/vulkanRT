#pragma once
#include "UniformBufferObject.h"

class UniformBufferShaderBindingTable : public UniformBufferObject
{
public:
	UniformBufferShaderBindingTable();
	~UniformBufferShaderBindingTable();

	void update(LogicalDevice& logicalDevice, const void* src);
};