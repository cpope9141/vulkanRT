#pragma once
#include "VertexData.h"

#include "QuadBufferObject.h"
#include "ShortBufferObject.h"
#include "vulkan/vulkan_core.h"

#include <vector>

VkVertexInputBindingDescription getBindingDescription();
std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

class QuadVertexData : public VertexData
{
public: 
	QuadVertexData();
	~QuadVertexData();

	void init(LogicalDevice logicalDevice, CommandPool commandPool);

private:
	ShortBufferObject indexBuffer;
	QuadBufferObject vertexBuffer;
};