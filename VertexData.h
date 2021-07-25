#pragma once
#include "LogicalDevice.h"
#include "TransferBufferObject.h"
#include "vulkan/vulkan_core.h"

class VertexData
{
public:
	VertexData();
	~VertexData();

    void destroy(LogicalDevice& logicalDevice);
	VkBuffer getIndexBuffer();
	VkDeviceAddress getIndexBufferDeviceAddress();
	uint32_t getIndexCount();
	VkIndexType getIndexType();
	VkBuffer getVertexBuffer();
	VkDeviceAddress getVertexBufferDeviceAddress();
	uint32_t getVertexCount();
	void setDeviceAddresses(LogicalDevice& logicalDevice);

protected:
	VkDeviceAddress indexBufferDeviceAddress;
    TransferBufferObject* indexBufferObject;
	uint32_t indexCount;
    VkIndexType indexType;
	VkDeviceAddress vertexBufferDeviceAddress;
    TransferBufferObject* vertexBufferObject;
	uint32_t vertexCount;

	VkDeviceAddress getBufferDeviceAddress(LogicalDevice logicalDevice, VkBuffer buffer);
};
