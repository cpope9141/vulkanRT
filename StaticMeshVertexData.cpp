#include "StaticMeshVertexData.h"

#include "Types.h"

#include <iostream>

//public
StaticMeshVertexData::StaticMeshVertexData()
{
    indexBufferObject = &indexBuffer;
    indexType = VK_INDEX_TYPE_UINT32;
    vertexBufferObject = &vertexBuffer;
}

StaticMeshVertexData::~StaticMeshVertexData() {}

std::vector<VkVertexInputAttributeDescription> StaticMeshVertexData::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    attributeDescriptions.resize(4);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(StaticMeshVertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(StaticMeshVertex, normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(StaticMeshVertex, tangent);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(StaticMeshVertex, texelCoords);

    return attributeDescriptions;
}

VkVertexInputBindingDescription StaticMeshVertexData::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription = {};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(StaticMeshVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

void StaticMeshVertexData::init(LogicalDevice logicalDevice, CommandPool commandPool, ModelResource& modelResource)
{
    indexBuffer.create(logicalDevice, commandPool, modelResource.indices.data(), modelResource.indices.size() * sizeof(uint32_t));
    vertexBuffer.create(logicalDevice, commandPool, modelResource.vertices.data(), modelResource.vertices.size() * sizeof(StaticMeshVertex));

    indexCount = modelResource.indices.size();
}