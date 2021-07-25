#include "UniformBufferRT.h"

//public
UniformBufferRT::UniformBufferRT()
{
    size = sizeof(RT);
}

UniformBufferRT::~UniformBufferRT() {}

void UniformBufferRT::update(LogicalDevice& logicalDevice, glm::mat4 view, glm::mat4 projection, glm::vec3 lightPosition, uint32_t vertexSize)
{
    void* ptr = nullptr;
    RT rt;
    rt.viewInverse = glm::inverse(view);
    rt.projInverse = glm::inverse(projection);
    rt.lightPos = lightPosition;
    rt.vertexSize = vertexSize;

    vkMapMemory(logicalDevice.getDevice(), bufferMemory, 0, getSize(), 0, &ptr);

    std::memcpy(ptr, &rt, sizeof(RT));

    vkUnmapMemory(logicalDevice.getDevice(), bufferMemory);
}