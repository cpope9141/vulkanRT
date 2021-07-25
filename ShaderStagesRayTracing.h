#pragma once
#include "ShaderStages.h"

class ShaderStagesRayTracing : public ShaderStages
{
public:
	ShaderStagesRayTracing();
	~ShaderStagesRayTracing();

	virtual void destroy(LogicalDevice& logicalDevice) override;

protected:
	VkShaderModule closestHitShaderModule;
	VkShaderModule missShaderModule;
	VkShaderModule raygenShaderModule;

	virtual const char* getShaderPathClosestHit() = 0;
	virtual const char* getShaderPathMiss() = 0;
	virtual const char* getShaderPathRaygen() = 0;
};