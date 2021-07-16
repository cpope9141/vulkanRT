#include "UniformBufferPBRLighting.h"

//public
UniformBufferPBRLighting::UniformBufferPBRLighting()
{
    size = sizeof(PBRLighting);
}

UniformBufferPBRLighting::~UniformBufferPBRLighting() {}

void UniformBufferPBRLighting::update(LogicalDevice logicalDevice, DirectionalLight directionalLight, PositionalLight positionalLights[2])
{
    void* ptr = nullptr;
    PBRLighting pbrLighting = {};
    pbrLighting.directionalLight = directionalLight;
    pbrLighting.positionalLights[0] = positionalLights[0];
    pbrLighting.positionalLights[1] = positionalLights[1];

    vkMapMemory(logicalDevice.getDevice(), bufferMemory, 0, getSize(), 0, &ptr);

    std::memcpy(ptr, &pbrLighting, sizeof(PBRLighting));

    vkUnmapMemory(logicalDevice.getDevice(), bufferMemory);
}