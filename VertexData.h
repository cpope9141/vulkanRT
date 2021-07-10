#pragma once
#include "LogicalDevice.h"
#include "TransferBufferObject.h"
#include "vulkan/vulkan_core.h"

class VertexData
{
public:
	VertexData();
	~VertexData();

    void destroy(LogicalDevice logicalDevice);
	VkBuffer getIndexBuffer();
	uint32_t getIndexCount();
	VkIndexType getIndexType();
	VkBuffer getVertexBuffer();

protected:
    TransferBufferObject* indexBufferObject;
	uint32_t indexCount;
    VkIndexType indexType;
    TransferBufferObject* vertexBufferObject;
};
