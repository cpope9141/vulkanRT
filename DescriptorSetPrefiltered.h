#pragma once
#include "DescriptorSet.h"

#include "CubeMap.h"
#include "GraphicsPipelinePrefiltered.h"

class DescriptorSetPrefiltered : public DescriptorSet
{
public:
	DescriptorSetPrefiltered();
	~DescriptorSetPrefiltered();

	void create(LogicalDevice logicalDevice, GraphicsPipelinePrefiltered* gp, CubeMap* cubeMap);

protected:
	virtual void updateDescriptorSet(LogicalDevice logicalDevice) override;

private:
	CubeMap* cubeMap;
};