#include "QuadVertexData.h"

#include "Types.h"

const uint16_t INDICES[] = { 0, 1, 3, 3, 1, 2 };
const std::vector<Vertex> VERTICES =
{
    {{-0.5f, 0.5f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f}, {1.0f, 0.0f}}
};

//public
QuadVertexData::QuadVertexData()
{
    indexBufferObject = &indexBuffer;
    indexCount = 6;
    indexType = VK_INDEX_TYPE_UINT16;
    vertexBufferObject = &vertexBuffer;
}

QuadVertexData::~QuadVertexData() {}

std::vector<VkVertexInputAttributeDescription> QuadVertexData::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    attributeDescriptions.resize(2);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, texelCoords);

    return attributeDescriptions;
}

VkVertexInputBindingDescription QuadVertexData::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription = {};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

void QuadVertexData::init(LogicalDevice logicalDevice, CommandPool commandPool)
{
    indexBuffer.create(logicalDevice, commandPool, INDICES, 6 * sizeof(uint16_t));
    vertexBuffer.create(logicalDevice, commandPool, VERTICES.data(), VERTICES.size() * sizeof(Vertex));
}