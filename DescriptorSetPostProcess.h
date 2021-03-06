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

	void create(LogicalDevice& logicalDevice, GraphicsPipeline* gp, UniformBufferObject* ubo, Texture* texture);
	void destroy(LogicalDevice& logicalDevice, GraphicsPipeline* gp);

protected:
	virtual void updateDescriptorSet(const LogicalDevice& logicalDevice) override;

private:
	Texture* texture;
	UniformBufferObject* ubo;
};