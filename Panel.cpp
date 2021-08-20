#include "Panel.h"

//public
Panel::Panel()
{
	vertexData = &quadVertexData;
}

Panel::~Panel() {}

void Panel::deinit(const LogicalDevice& logicalDevice)
{
	vertexData->destroy(logicalDevice);
}

void Panel::init(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
{
	quadVertexData.init(logicalDevice, commandPool);
}