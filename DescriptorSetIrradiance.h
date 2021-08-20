#pragma once
#include "DescriptorSet.h"

#include "CubeMap.h"
#include "GraphicsPipelineIrradiance.h"

class DescriptorSetIrradiance : public DescriptorSet
{
public:
	DescriptorSetIrradiance();
	~DescriptorSetIrradiance();

	void create(const LogicalDevice& logicalDevice, GraphicsPipelineIrradiance* gp, CubeMap* cubeMap);

protected:
	virtual void updateDescriptorSet(const LogicalDevice& logicalDevice) override;

private:
	CubeMap* cubeMap;
};