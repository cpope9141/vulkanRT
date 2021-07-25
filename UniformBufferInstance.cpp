#include "UniformBufferInstance.h"

//public
UniformBufferInstance::UniformBufferInstance() {}
UniformBufferInstance::~UniformBufferInstance() {}

void UniformBufferInstance::update(LogicalDevice& logicalDevice, std::vector<VkAccelerationStructureInstanceKHR>& instances)
{
    void* ptr = nullptr;

    vkMapMemory(logicalDevice.getDevice(), bufferMemory, 0, getSize(), 0, &ptr);

    std::memcpy(ptr, instances.data(), instances.size() * sizeof(VkAccelerationStructureInstanceKHR));

    vkUnmapMemory(logicalDevice.getDevice(), bufferMemory);
}