#pragma once
#include "ShaderStagesRayTracing.h"

class ShaderStagesRTPBR : public ShaderStagesRayTracing
{
public:
	ShaderStagesRTPBR();
	~ShaderStagesRTPBR();

	virtual void destroy(LogicalDevice& logicalDevice) override;
	virtual std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(LogicalDevice& logicalDevice) override;

protected:
	virtual const char* getShaderPathClosestHit() override;
	virtual const char* getShaderPathMiss() override;
	virtual const char* getShaderPathRaygen() override;

private:
	VkShaderModule missShadowShaderModule;

	const char* SHADER_PATH_CLOSEST_HIT = "Shaders/closestHit.rchit.spv";
	const char* SHADER_PATH_MISS = "Shaders/miss.rmiss.spv";
	const char* SHADER_PATH_RAYGEN = "Shaders/raygen.rgen.spv";
	const char* SHADER_PATH_MISS_SHADOW = "Shaders/shadow.rmiss.spv";
};