#pragma once
#include "DescriptorSet.h"

#include "Texture.h"
#include "UniformBufferObject.h" 

class DescriptorSetPostProcess : public DescriptorSet
{
public:
	DescriptorSetPostProcess();
	~DescriptorSetPostProcess();

protected:
	virtual std::vector<VkWriteDescriptorSet> writeDescriptorSets(LogicalDevice logicalDevice) override;

private:
	Texture* texture;
	UniformBufferObject* ubo;
};