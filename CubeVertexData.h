#pragma once
#include "VertexData.h"

#include <vector>

class CubeVertexData : public VertexData
{
public:
	CubeVertexData();
	~CubeVertexData();

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	static VkVertexInputBindingDescription getBindingDescription();

	void init(LogicalDevice logicalDevice, CommandPool commandPool);
};