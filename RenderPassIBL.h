#pragma once
#include "RenderPass.h"

class RenderPassIBL : public RenderPass
{
public:
	RenderPassIBL();
	~RenderPassIBL();

	virtual void create(LogicalDevice logicalDevice, VkFormat imageFormat) override;
};