#pragma once
#include "DescriptorSet.h"

#include "GraphicsPipeline.h"
#include "LogicalDevice.h"
#include "Texture.h"
#include "UniformBufferObject.h" 

class DescriptorSetPostProcess : public DescriptorSet
{
public:
	DescriptorSetPostProcess();
	~DescriptorSetPostProcess();

	void create(LogicalDevice logicalDevice, GraphicsPipeline* gp, UniformBufferObject* ubo);

protected:
	virtual void updateDescriptorSet(LogicalDevice logicalDevice) override;

private:
	Texture* texture;
	UniformBufferObject* ubo;
};