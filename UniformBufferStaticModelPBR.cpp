#include "UniformBufferStaticModelPBR.h"

struct UBOStaticModelPBR
{
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 modelMatrix;
    alignas(16) glm::mat4 normalMatrix;
};

//public
UniformBufferStaticModelPBR::UniformBufferStaticModelPBR()
{
    size = sizeof(UBOStaticModelPBR);
}

UniformBufferStaticModelPBR::~UniformBufferStaticModelPBR() {}

void UniformBufferStaticModelPBR::update(LogicalDevice& logicalDevice, glm::mat4 viewMatrix, glm::mat4 modelMatrix, glm::mat4 normalMatrix)
{
    void* ptr = nullptr;
    UBOStaticModelPBR ubo = {};
    ubo.viewMatrix = viewMatrix;
    ubo.modelMatrix = modelMatrix;
    ubo.normalMatrix = normalMatrix;

    vkMapMemory(logicalDevice.getDevice(), bufferMemory, 0, getSize(), 0, &ptr);

    std::memcpy(ptr, &ubo, sizeof(UBOStaticModelPBR));

    vkUnmapMemory(logicalDevice.getDevice(), bufferMemory);
}