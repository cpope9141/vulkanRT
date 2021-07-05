#pragma once
#include "ShaderSPIRV.h"
#include "ShaderStages.h"

class ShaderStagesGraphics : public ShaderStages
{
public:
	ShaderStagesGraphics();
	~ShaderStagesGraphics();

	virtual void destroy(LogicalDevice logicalDevice) override;
	virtual std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos(LogicalDevice logicalDevice) override;

protected:
	VkShaderModule fragmentShaderModule;
	VkShaderModule vertexShaderModule;

	virtual const char* getShaderPathFragment() = 0;
	virtual const char* getShaderPathVertex() = 0;
};