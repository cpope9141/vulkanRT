#pragma once
#include "DescriptorSet.h"

#include "GraphicsPipelineIntegratedBRDF.h"
#include "UniformBufferProjection.h"

class DescriptorSetIntegratedBRDF : public DescriptorSet
{
public:
	DescriptorSetIntegratedBRDF();
	~DescriptorSetIntegratedBRDF();

	void create(LogicalDevice logicalDevice, GraphicsPipelineIntegratedBRDF* gp, UniformBufferProjection* ubo);

protected:
	virtual void updateDescriptorSet(LogicalDevice logicalDevice) override;

private:
	UniformBufferProjection* ubo;
};