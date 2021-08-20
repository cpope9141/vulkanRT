#include "VertexData.h"

//public
VertexData::VertexData()
{
	indexBufferDeviceAddress = VK_NULL_HANDLE;
	indexBufferObject = nullptr;
	indexCount = 0;
	indexType = VK_INDEX_TYPE_UINT32;
	vertexBufferDeviceAddress = VK_NULL_HANDLE;
	vertexBufferObject = nullptr;
	vertexCount = 0;
}

VertexData::~VertexData() {}

void VertexData::destroy(const LogicalDevice& logicalDevice)
{
	indexBufferObject->destroy(logicalDevice);
	vertexBufferObject->destroy(logicalDevice);
}

VkBuffer VertexData::getIndexBuffer() { return indexBufferObject->getBuffer(); }
VkDeviceAddress VertexData::getIndexBufferDeviceAddress() { return indexBufferDeviceAddress; }
uint32_t VertexData::getIndexCount() { return indexCount; }
VkIndexType VertexData::getIndexType() { return indexType; }
VkBuffer VertexData::getVertexBuffer() { return vertexBufferObject->getBuffer(); }
VkDeviceAddress VertexData::getVertexBufferDeviceAddress() { return vertexBufferDeviceAddress; }
uint32_t VertexData::getVertexCount() { return vertexCount; }

void VertexData::setDeviceAddresses(const LogicalDevice& logicalDevice)
{
	indexBufferDeviceAddress = getBufferDeviceAddress(logicalDevice, getIndexBuffer());
	vertexBufferDeviceAddress = getBufferDeviceAddress(logicalDevice, getVertexBuffer());
}

//protected
VkDeviceAddress VertexData::getBufferDeviceAddress(const LogicalDevice& logicalDevice, VkBuffer buffer)
{
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR =
		reinterpret_cast<PFN_vkGetBufferDeviceAddressKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetBufferDeviceAddressKHR"));
	VkBufferDeviceAddressInfoKHR bufferDeviceAI = {};

	bufferDeviceAI.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	bufferDeviceAI.buffer = buffer;

	return vkGetBufferDeviceAddressKHR(logicalDevice.getDevice(), &bufferDeviceAI);
}