#pragma once
#include "RayTracingPipeline.h"

#include "ShaderStagesRTPBR.h"

class RayTracingPipelinePBR : public RayTracingPipeline
{
public:
	RayTracingPipelinePBR();
	~RayTracingPipelinePBR();

	virtual uint32_t getGroupCount() override;
	virtual uint8_t getPushConstantSize() override;
	virtual VkShaderStageFlags getPushConstantStages() override;

protected:
	virtual void createDescriptorPool(LogicalDevice& logicalDevice) override;
	virtual void createDescriptorSetLayout(LogicalDevice& logicalDevice) override;
	virtual void createPipelineLayout(LogicalDevice& logicalDevice) override;
	virtual std::vector<VkRayTracingShaderGroupCreateInfoKHR> createShaderGroups(LogicalDevice& logicalDevice) override;

private:
	ShaderStagesRTPBR shaderStagesRTPBR;
};