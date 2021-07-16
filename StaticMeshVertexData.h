#pragma once
#include "VertexData.h"

#include "IntegerBufferObject.h"
#include "StaticMeshBufferObject.h"
#include "Types.h"
#include "vulkan/vulkan_core.h"

#include <vector>

class StaticMeshVertexData : public VertexData
{
public:
	StaticMeshVertexData();
	~StaticMeshVertexData();

	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	static VkVertexInputBindingDescription getBindingDescription();

	void init(LogicalDevice logicalDevice, CommandPool commandPool, ModelResource& modelResource);

private:
	IntegerBufferObject indexBuffer;
	StaticMeshBufferObject vertexBuffer;
};