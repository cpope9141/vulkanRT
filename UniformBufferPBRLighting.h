#pragma once
#include "UniformBufferObject.h"

#include "Types.h"

class UniformBufferPBRLighting : public UniformBufferObject
{
public:
	UniformBufferPBRLighting();
	~UniformBufferPBRLighting();

	void update(LogicalDevice logicalDevice, DirectionalLight directionalLight, PositionalLight positionalLights[2]);
};