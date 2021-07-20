#pragma once
#include "VertexData.h"

#include "Types.h"

#include <vector>

class StaticMeshVertexData : public VertexData
{
public:
	StaticMeshVertexData();
	~StaticMeshVertexData();

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	static VkVertexInputBindingDescription getBindingDescription();

	void init(LogicalDevice& logicalDevice, CommandPool& commandPool, ModelResource& modelResource);
};