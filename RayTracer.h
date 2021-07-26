#pragma once
#include "AccelerationStructure.h"
#include "CommandPool.h"
#include "Cube.h"
#include "DescriptorSetRTPBR.h"
#include "LogicalDevice.h"
#include "ModelRT.h"
#include "PrecomputedIBL.h"
#include "RayTracingPipelinePBR.h"
#include "RayTracingScratchBuffer.h"
#include "SceneStorageBuffer.h"
#include "UniformBufferRT.h"
#include "UniformBufferShaderBindingTable.h"
#include "vulkan/vulkan_core.h"

class RayTracer
{
public:
	RayTracer();
	~RayTracer();

	void create(LogicalDevice& logicalDevice, CommandPool& commandPool, PrecomputedIBL& precomputedIBL, std::vector<ModelRT>& models, glm::mat4 view, glm::mat4 proj, glm::vec3 lightPos, size_t swapChainImagesCount);
	void destroy(LogicalDevice& logicalDevice);
	void draw(LogicalDevice& logicalDevice, CommandBuffer& commandBuffer, uint32_t nextImageIndex);
	std::vector<Texture>& getOutputImages();

private:
	VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures;
	std::vector<AccelerationStructure> blases;
	Cube cube;
	DescriptorSetRTPBR descriptorSetRTPBR;
	std::vector<Texture> outputImages;
	RayTracingPipelinePBR rayTracingPipelinePBR;
	VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingPipelineProperties;
	SceneStorageBuffer sceneStorageBuffer;
	Texture storageImage = Texture(VK_FILTER_LINEAR);
	AccelerationStructure tlas = AccelerationStructure(VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR);
	UniformBufferRT uboRT;
	UniformBufferShaderBindingTable uboSBTClosestHit;
	UniformBufferShaderBindingTable uboSBTMiss;
	UniformBufferShaderBindingTable uboSBTRaygen;	

	uint32_t alignedSize(uint32_t value, uint32_t alignment);
	void buildAccelerationStructure(LogicalDevice& logicalDevice,
		CommandPool& commandPool,
		VkAccelerationStructureBuildGeometryInfoKHR asBuildGeometryInfo,
		std::vector<VkAccelerationStructureBuildRangeInfoKHR*>& list);
	AccelerationStructure createBottomLevelAccelerationStructure(LogicalDevice& logicalDevice, CommandPool& commandPool, Model& model);
	RayTracingScratchBuffer createScratchBuffer(LogicalDevice& logicalDevice, VkDeviceSize size);
	void createShaderBindingTable(LogicalDevice& logicalDevice);
	void createStorageImages(LogicalDevice& logicalDevice, CommandPool& commandPool, size_t swapChainImagesCount);
	void createTopLevelAccelerationStructure(LogicalDevice& logicalDevice, CommandPool& commandPool);
	void destroyShaderBindingTable(LogicalDevice& logicalDevice);
	void destroyStorageImage(LogicalDevice& logicalDevice);
	VkDeviceAddress getBufferDeviceAddress(LogicalDevice& logicalDevice, VkBuffer buffer);
};