#include "CubeVertexData.h"

#include "Types.h"

const std::vector<uint16_t> INDICES = {
        0,1,3,
        3,1,2,
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22
};

const std::vector<CubeVertex> VERTICES = {
    //front
    {{-1, 1, 1}, {0, 0}},    
    {{-1, -1, 1}, {0, 1}},
    {{1, -1, 1}, {1, 1}},
    {{1, 1, 1}, {1, 0}},

    //back
    {{1, 1, -1}, {0, 0}},
    {{1, -1, -1}, {0, 1}},
    {{-1, -1, -1}, {1, 1}},
    {{-1, 1, -1}, {1, 0}},            

    //right
    {{1, 1, 1}, {0, 0}},
    {{1, -1, 1}, {0, 1}},
    {{1, -1, -1}, {1, 1}},
    {{1, 1, -1}, {1, 0}},

    //left
    {{-1, 1, -1}, {0, 0}},
    {{-1, -1, -1}, {0, 1}},
    {{-1, -1, 1}, {1, 1}},
    {{-1, 1, 1}, {1, 0}},

    //top
    {{-1, 1, -1}, {0, 0}},
    {{-1, 1, 1}, {0, 1}},
    {{1, 1, 1}, {1, 1}},
    {{1, 1, -1}, {1, 0}},

    //bottom
    {{-1, -1, 1}, {0, 0}},
    {{-1, -1, -1}, {0, 1}},
    {{1, -1, -1}, {1, 1}},
    {{1, -1, 1}, {1, 0}}
};

//public
CubeVertexData::CubeVertexData()
{
    indexBufferObject = new TransferBufferObject(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    indexCount = static_cast<uint32_t>(INDICES.size());
    indexType = VK_INDEX_TYPE_UINT16;
    vertexBufferObject = new TransferBufferObject(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
}

CubeVertexData::~CubeVertexData()
{
    delete indexBufferObject;
    delete vertexBufferObject;
}

std::vector<VkVertexInputAttributeDescription> CubeVertexData::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};

    attributeDescriptions.resize(2);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(CubeVertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(CubeVertex, texelCoords);

    return attributeDescriptions;
}

VkVertexInputBindingDescription CubeVertexData::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription = {};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(CubeVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

void CubeVertexData::init(LogicalDevice logicalDevice, CommandPool commandPool)
{
    indexBufferObject->create(logicalDevice, commandPool, INDICES.data(), INDICES.size() * sizeof(uint16_t));
    vertexBufferObject->create(logicalDevice, commandPool, VERTICES.data(), VERTICES.size() * sizeof(CubeVertex));
}