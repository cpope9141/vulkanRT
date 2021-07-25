#pragma once
#include "LogicalDevice.h"
#include "Model.h"
#include "vulkan/vulkan_core.h"

class AccelerationStructure
{
public:
    AccelerationStructure();
	AccelerationStructure(VkAccelerationStructureTypeKHR type);
	~AccelerationStructure();

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    uint64_t deviceAddress;
    VkAccelerationStructureKHR handle;
    Model* referenceModel;
    VkAccelerationStructureTypeKHR type;

    void createAccelerationStructureBuffer(LogicalDevice& logicalDevice, VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo);
    void destroy(LogicalDevice& logicalDevice);
};