#include "AccelerationStructure.h"

#include "MemoryUtilities.h"

#include <iostream>

//public
AccelerationStructure::AccelerationStructure() {}

AccelerationStructure::AccelerationStructure(VkAccelerationStructureTypeKHR type)
{
    buffer = VK_NULL_HANDLE;
    bufferMemory = VK_NULL_HANDLE;
    deviceAddress = 0;
    handle = VK_NULL_HANDLE;
    referenceModel = nullptr;
	this->type = type;
}

AccelerationStructure::~AccelerationStructure() {}

void AccelerationStructure::createAccelerationStructureBuffer(LogicalDevice& logicalDevice, VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo)
{
	PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR =
		reinterpret_cast<PFN_vkCreateAccelerationStructureKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkCreateAccelerationStructureKHR"));

	VkAccelerationStructureCreateInfoKHR accelerationStructureCreateInfo = {};

	createBuffer(logicalDevice,
		buildSizesInfo.accelerationStructureSize,
		VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&buffer,
		&bufferMemory);

	accelerationStructureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	accelerationStructureCreateInfo.buffer = buffer;
	accelerationStructureCreateInfo.size = buildSizesInfo.accelerationStructureSize;
	accelerationStructureCreateInfo.type = type;

	if (VK_SUCCESS == vkCreateAccelerationStructureKHR(logicalDevice.getDevice(), &accelerationStructureCreateInfo, nullptr, &handle))
	{
		PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR =
			reinterpret_cast<PFN_vkGetAccelerationStructureDeviceAddressKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetAccelerationStructureDeviceAddressKHR"));

		VkAccelerationStructureDeviceAddressInfoKHR accelerationDeviceAddressInfo = {};
		accelerationDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
		accelerationDeviceAddressInfo.accelerationStructure = handle;
		deviceAddress = vkGetAccelerationStructureDeviceAddressKHR(logicalDevice.getDevice(), &accelerationDeviceAddressInfo);
	}
	else
	{
		throw std::runtime_error("Failed to create acceleration structure");
	}
}

void AccelerationStructure::destroy(LogicalDevice& logicalDevice)
{
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR =
        reinterpret_cast<PFN_vkDestroyAccelerationStructureKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkDestroyAccelerationStructureKHR"));

    vkDestroyAccelerationStructureKHR(logicalDevice.getDevice(), handle, nullptr);

    vkDestroyBuffer(logicalDevice.getDevice(), buffer, nullptr);
    vkFreeMemory(logicalDevice.getDevice(), bufferMemory, nullptr);
}