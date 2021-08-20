#pragma once
#include "RenderPass.h"

class RenderPassPresentation : public RenderPass
{
public:
	RenderPassPresentation();
	~RenderPassPresentation();

	virtual void create(const LogicalDevice& logicalDevice, VkFormat imageFormat) override;
};