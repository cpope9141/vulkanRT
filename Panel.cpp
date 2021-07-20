#include "Panel.h"

//public
Panel::Panel()
{
	vertexData = &quadVertexData;
}

Panel::~Panel() {}

void Panel::deinit(LogicalDevice& logicalDevice)
{
	vertexData->destroy(logicalDevice);
}

void Panel::init(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
	quadVertexData.init(logicalDevice, commandPool);
}