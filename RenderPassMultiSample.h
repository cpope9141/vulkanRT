#pragma once
#include "RenderPass.h"

class RenderPassMultiSample : public RenderPass
{
public:
	RenderPassMultiSample();
	~RenderPassMultiSample();

	virtual void create(LogicalDevice logicalDevice, VkFormat imageFormat) override;
};