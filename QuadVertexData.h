#pragma once
#include "VertexData.h"

#include <vector>

class QuadVertexData : public VertexData
{
public: 
	QuadVertexData();
	~QuadVertexData();

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	static VkVertexInputBindingDescription getBindingDescription();

	void init(const LogicalDevice& logicalDevice, const CommandPool& commandPool);
};