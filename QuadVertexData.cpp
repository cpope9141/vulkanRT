#include "QuadVertexData.h"

#include "Types.h"

const std::vector<uint16_t> INDICES = { 0, 1, 3, 3, 1, 2 };
const std::vector<QuadVertex> VERTICES =
{
    {{-0.5f, 0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f}, {1.0f, 0.0f}}
};

//public
QuadVertexData::QuadVertexData()
{
    indexBufferObject = new TransferBufferObject(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    indexCount = INDICES.size();
    indexType = VK_INDEX_TYPE_UINT16;
    vertexBufferObject = new TransferBufferObject(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
}

QuadVertexData::~QuadVertexData()
{
    delete indexBufferObject;
    delete vertexBufferObject;
}

std::vector<VkVertexInputAttributeDescription> QuadVertexData::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    attributeDescriptions.resize(2);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(QuadVertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(QuadVertex, texelCoords);

    return attributeDescriptions;
}

VkVertexInputBindingDescription QuadVertexData::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription = {};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(QuadVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

void QuadVertexData::init(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
    indexBufferObject->create(logicalDevice, commandPool, INDICES.data(), INDICES.size() * sizeof(uint16_t));
    vertexBufferObject->create(logicalDevice, commandPool, VERTICES.data(), VERTICES.size() * sizeof(QuadVertex));
}