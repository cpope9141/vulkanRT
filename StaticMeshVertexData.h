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

	void init(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const ModelResource& modelResource);
};