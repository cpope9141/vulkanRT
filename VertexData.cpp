#include "VertexData.h"

//public
VertexData::VertexData()
{
	indexBufferObject = nullptr;
	indexCount = 0;
	indexType = VK_INDEX_TYPE_UINT32;
	vertexBufferObject = nullptr;
}

VertexData::~VertexData() {}

void VertexData::destroy(LogicalDevice& logicalDevice)
{
	indexBufferObject->destroy(logicalDevice);
	vertexBufferObject->destroy(logicalDevice);
}

VkBuffer VertexData::getIndexBuffer() { return indexBufferObject->getBuffer(); }
uint32_t VertexData::getIndexCount() { return indexCount; }
VkIndexType VertexData::getIndexType() { return indexType; }
VkBuffer VertexData::getVertexBuffer() { return vertexBufferObject->getBuffer(); }