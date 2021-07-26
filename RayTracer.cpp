#include "RayTracer.h"

#include "MemoryUtilities.h"
#include "RayTracingScratchBuffer.h"
#include "Types.h"
#include "UniformBufferInstance.h"
#include "WindowManager.h"

#include <iostream>

//public
RayTracer::RayTracer()
{
    accelerationStructureFeatures = {};
    rayTracingPipelineProperties = {};
}

RayTracer::~RayTracer() {}

void RayTracer::create(LogicalDevice& logicalDevice, CommandPool& commandPool, PrecomputedIBL& precomputedIBL, std::vector<ModelRT>& models, glm::mat4 view, glm::mat4 proj, glm::vec3 lightPos)
{
    VkPhysicalDevice physicalDevice = logicalDevice.getPhysicalDevicePtr()->getPhysicalDevice();
    VkPhysicalDeviceProperties2 deviceProperties2 = {};
    VkPhysicalDeviceFeatures2 deviceFeatures2 = {};

    rayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
    deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    deviceProperties2.pNext = &rayTracingPipelineProperties;
    vkGetPhysicalDeviceProperties2(physicalDevice, &deviceProperties2);

    accelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    deviceFeatures2.pNext = &accelerationStructureFeatures;
    vkGetPhysicalDeviceFeatures2(physicalDevice, &deviceFeatures2);

	blases.clear();
	blases.resize(models.size());
	for (size_t i = 0; i < blases.size(); i++) { blases[i] = createBottomLevelAccelerationStructure(logicalDevice, commandPool, models[i]); }

    createTopLevelAccelerationStructure(logicalDevice, commandPool);

    createStorageImage(logicalDevice, commandPool);

    uboRT.create(logicalDevice);
	uboRT.update(logicalDevice, view, proj, lightPos, sizeof(StaticMeshVertex));

	sceneStorageBuffer.create(logicalDevice, commandPool, models);

    rayTracingPipelinePBR.create(logicalDevice);

    createShaderBindingTable(logicalDevice);
	std::vector<Texture*> textures;
	textures.resize(models.size() * 5);
	textures[0] = models[0].getAlbedoPtr();
	textures[1] = models[0].getAmbientOcclusionPtr();
	textures[2] = models[0].getMetallicPtr();
	textures[3] = models[0].getNormalPtr();
	textures[4] = models[0].getRoughnessPtr();

	cube.init(logicalDevice, commandPool);
    descriptorSetRTPBR.create(logicalDevice, &rayTracingPipelinePBR, &storageImage, &tlas, &uboRT, &sceneStorageBuffer, textures, &precomputedIBL, cube.getCubeMap());
}

void RayTracer::destroy(LogicalDevice& logicalDevice)
{
	cube.deinit(logicalDevice);
	destroyShaderBindingTable(logicalDevice);

	rayTracingPipelinePBR.destroy(logicalDevice);

	sceneStorageBuffer.destroy(logicalDevice);
	uboRT.destroy(logicalDevice);

	destroyStorageImage(logicalDevice);

	tlas.destroy(logicalDevice);

	for (size_t i = 0; i < blases.size(); i++) { blases[i].destroy(logicalDevice); }
}

void RayTracer::draw(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
	PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR =
		reinterpret_cast<PFN_vkCmdTraceRaysKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkCmdTraceRaysKHR"));

	const uint32_t handleSizeAligned = alignedSize(rayTracingPipelineProperties.shaderGroupHandleSize, rayTracingPipelineProperties.shaderGroupHandleAlignment);
	CommandBuffer commandBuffer(commandPool);
	VkDescriptorSet ds = descriptorSetRTPBR.getDescriptorSet();

	VkStridedDeviceAddressRegionKHR hitShaderSbtEntry = {};
	VkStridedDeviceAddressRegionKHR missShaderSbtEntry = {};
	VkStridedDeviceAddressRegionKHR raygenShaderSbtEntry = {};
	VkStridedDeviceAddressRegionKHR callableShaderSbtEntry = {};

	hitShaderSbtEntry.deviceAddress = getBufferDeviceAddress(logicalDevice, uboSBTClosestHit.getBuffer());
	hitShaderSbtEntry.stride = handleSizeAligned;
	hitShaderSbtEntry.size = handleSizeAligned;

	missShaderSbtEntry.deviceAddress = getBufferDeviceAddress(logicalDevice, uboSBTMiss.getBuffer());
	missShaderSbtEntry.stride = handleSizeAligned;
	missShaderSbtEntry.size = handleSizeAligned;

	raygenShaderSbtEntry.deviceAddress = getBufferDeviceAddress(logicalDevice, uboSBTRaygen.getBuffer());
	raygenShaderSbtEntry.stride = handleSizeAligned;
	raygenShaderSbtEntry.size = handleSizeAligned;

	commandBuffer.beginOneTimeCommandBuffer(logicalDevice);

	vkCmdBindPipeline(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, rayTracingPipelinePBR.getPipeline());
	vkCmdBindDescriptorSets(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, rayTracingPipelinePBR.getPipelineLayout(), 0, 1, &ds, 0, nullptr);

	vkCmdTraceRaysKHR(commandBuffer.getCommandBuffer(),
		&raygenShaderSbtEntry,
		&missShaderSbtEntry,
		&hitShaderSbtEntry,
		&callableShaderSbtEntry,
		storageImage.getWidth(),
		storageImage.getHeight(),
		1);

	outputImage.getImage().transitionLayout(logicalDevice, commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, false);

	storageImage.getImage().transitionLayout(logicalDevice, commandBuffer, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, false);

	storageImage.getImage().copyToImage(outputImage.getImage().getImage(), outputImage.getHeight(), outputImage.getWidth(), 0, 0, commandBuffer);

	storageImage.getImage().transitionLayout(logicalDevice, commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, false);

	outputImage.getImage().transitionLayout(logicalDevice, commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, false);

	commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
}

Texture* RayTracer::getOutputImage() { return &outputImage; }

//private
uint32_t RayTracer::alignedSize(uint32_t value, uint32_t alignment) { return (value + alignment - 1) & ~(alignment - 1); }

void RayTracer::buildAccelerationStructure(LogicalDevice& logicalDevice,
	CommandPool& commandPool,
	VkAccelerationStructureBuildGeometryInfoKHR asBuildGeometryInfo,
	std::vector<VkAccelerationStructureBuildRangeInfoKHR*>& list)
{
	if (accelerationStructureFeatures.accelerationStructureHostCommands)
	{
		PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR =
			reinterpret_cast<PFN_vkBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkBuildAccelerationStructuresKHR"));		
		vkBuildAccelerationStructuresKHR(
			logicalDevice.getDevice(),
			VK_NULL_HANDLE,
			1,
			&asBuildGeometryInfo,
			list.data());
	}
	else
	{
		PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR =
			reinterpret_cast<PFN_vkCmdBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkCmdBuildAccelerationStructuresKHR"));
		CommandBuffer commandBuffer(commandPool);
		commandBuffer.beginOneTimeCommandBuffer(logicalDevice);
		vkCmdBuildAccelerationStructuresKHR(
			commandBuffer.getCommandBuffer(),
			1,
			&asBuildGeometryInfo,
		    list.data());
		commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
	}
}

AccelerationStructure RayTracer::createBottomLevelAccelerationStructure(LogicalDevice& logicalDevice, CommandPool& commandPool, Model& model)
{
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR =
		reinterpret_cast<PFN_vkGetAccelerationStructureBuildSizesKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetAccelerationStructureBuildSizesKHR"));
	AccelerationStructure blas(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR);
	VertexData* vertexData = model.getVertexDataPtr();
	uint32_t primitiveCount = vertexData->getIndexCount() / 3;
	uint32_t maxVertex = vertexData->getVertexCount();

	VkDeviceOrHostAddressConstKHR vertexBufferDeviceAddress = {};
	VkDeviceOrHostAddressConstKHR indexBufferDeviceAddress = {};
	vertexBufferDeviceAddress.deviceAddress = vertexData->getVertexBufferDeviceAddress();
	indexBufferDeviceAddress.deviceAddress = vertexData->getIndexBufferDeviceAddress();

	VkAccelerationStructureGeometryKHR accelerationStructureGeometries = {};
	accelerationStructureGeometries.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	accelerationStructureGeometries.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
	accelerationStructureGeometries.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
	accelerationStructureGeometries.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	accelerationStructureGeometries.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
	accelerationStructureGeometries.geometry.triangles.vertexData = vertexBufferDeviceAddress;
	accelerationStructureGeometries.geometry.triangles.maxVertex = maxVertex;
	accelerationStructureGeometries.geometry.triangles.vertexStride = sizeof(StaticMeshVertex);
	accelerationStructureGeometries.geometry.triangles.indexType = vertexData->getIndexType();
	accelerationStructureGeometries.geometry.triangles.indexData = indexBufferDeviceAddress;

	VkAccelerationStructureBuildGeometryInfoKHR accelerationStructureBuildGeometryInfo = {};
	accelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	accelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	accelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
	accelerationStructureBuildGeometryInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
	accelerationStructureBuildGeometryInfo.geometryCount = 1;
	accelerationStructureBuildGeometryInfo.pGeometries = &accelerationStructureGeometries;
	accelerationStructureBuildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE;

	VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfo = {};
	accelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	vkGetAccelerationStructureBuildSizesKHR(logicalDevice.getDevice(),
		VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
		&accelerationStructureBuildGeometryInfo,
		&primitiveCount,
		&accelerationStructureBuildSizesInfo);

	blas.createAccelerationStructureBuffer(logicalDevice, accelerationStructureBuildSizesInfo);

	RayTracingScratchBuffer scratchBuffer = createScratchBuffer(logicalDevice, accelerationStructureBuildSizesInfo.buildScratchSize);

	accelerationStructureBuildGeometryInfo.dstAccelerationStructure = blas.handle;
	accelerationStructureBuildGeometryInfo.scratchData.deviceAddress = scratchBuffer.deviceAddress;

	VkAccelerationStructureBuildRangeInfoKHR accelerationStructureBuildRangeInfo = {};
	accelerationStructureBuildRangeInfo.primitiveCount = primitiveCount;
	accelerationStructureBuildRangeInfo.primitiveOffset = 0;
	accelerationStructureBuildRangeInfo.firstVertex = 0;
	accelerationStructureBuildRangeInfo.transformOffset = 0;

	std::vector<VkAccelerationStructureBuildRangeInfoKHR*> list = { &accelerationStructureBuildRangeInfo };

	buildAccelerationStructure(logicalDevice, commandPool, accelerationStructureBuildGeometryInfo, list);

	scratchBuffer.destroy(logicalDevice);

	blas.referenceModel = &model;

	return blas;
}

RayTracingScratchBuffer RayTracer::createScratchBuffer(LogicalDevice& logicalDevice, VkDeviceSize size)
{
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR =
		reinterpret_cast<PFN_vkGetBufferDeviceAddressKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetBufferDeviceAddressKHR"));
	RayTracingScratchBuffer scratchBuffer;
	VkBufferDeviceAddressInfoKHR bufferDeviceAddressInfo = {};

	createBuffer(logicalDevice,
		size,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&scratchBuffer.buffer,
		&scratchBuffer.bufferMemory);

	bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	bufferDeviceAddressInfo.buffer = scratchBuffer.buffer;

	scratchBuffer.deviceAddress = vkGetBufferDeviceAddressKHR(logicalDevice.getDevice(), &bufferDeviceAddressInfo);

	return scratchBuffer;
}

void RayTracer::createShaderBindingTable(LogicalDevice& logicalDevice)
{
	PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR =
		reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetRayTracingShaderGroupHandlesKHR"));

	const uint32_t handleSize = rayTracingPipelineProperties.shaderGroupHandleSize;
	const uint32_t handleSizeAligned = alignedSize(handleSize, rayTracingPipelineProperties.shaderGroupHandleAlignment);
	const uint32_t groupCount = rayTracingPipelinePBR.getGroupCount();
	const uint32_t sbtSize = groupCount * handleSizeAligned;

	std::vector<uint8_t> shaderHandleStorage(sbtSize);

	if (VK_SUCCESS == vkGetRayTracingShaderGroupHandlesKHR(logicalDevice.getDevice(), rayTracingPipelinePBR.getPipeline(), 0, groupCount, sbtSize, shaderHandleStorage.data()))
	{
		uboSBTRaygen.create(logicalDevice,
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			handleSize);
		uboSBTRaygen.update(logicalDevice, shaderHandleStorage.data());

		uboSBTMiss.create(logicalDevice,
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			2 * handleSize);
		uboSBTMiss.update(logicalDevice, shaderHandleStorage.data() + handleSizeAligned);

		uboSBTClosestHit.create(logicalDevice,
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			handleSize);
		uboSBTClosestHit.update(logicalDevice, shaderHandleStorage.data() +  3 * handleSizeAligned);
	}
	else
	{
		throw std::runtime_error("Failed to get shader group handles");
	}
}

void RayTracer::createStorageImage(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
	CommandBuffer commandBuffer(commandPool);

	outputImage.create(logicalDevice,
		HEIGHT,
		WIDTH,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		false,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK);

	storageImage.create(logicalDevice,
		HEIGHT,
		WIDTH,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		false,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK);

	commandBuffer.beginOneTimeCommandBuffer(logicalDevice);

	outputImage.getImage().transitionLayout(logicalDevice,
		commandBuffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		false);

	storageImage.getImage().transitionLayout(logicalDevice,
		commandBuffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_GENERAL,
		false);

	commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
}

void RayTracer::createTopLevelAccelerationStructure(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR =
		reinterpret_cast<PFN_vkGetAccelerationStructureBuildSizesKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetAccelerationStructureBuildSizesKHR"));

	std::vector<VkAccelerationStructureInstanceKHR> instances;
	instances.resize(blases.size());
	UniformBufferInstance uboInstance;

	for (size_t i = 0; i < instances.size(); i++)
	{
		glm::mat4 transpose = glm::transpose(blases[i].referenceModel->prepareModelMatrix());

		memcpy(&instances[i].transform, &transpose, sizeof(instances[i].transform));
		instances[i].instanceCustomIndex = static_cast<uint32_t>(i);
		instances[i].mask = 0xFF;
		instances[i].instanceShaderBindingTableRecordOffset = 0;
		instances[i].flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
		instances[i].accelerationStructureReference = blases[i].deviceAddress;
	}

	uboInstance.create(logicalDevice,
		VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR,
		instances.size() * sizeof(VkAccelerationStructureInstanceKHR));
	uboInstance.update(logicalDevice, instances);

	VkDeviceOrHostAddressConstKHR instanceDataDeviceAddress = {};
	instanceDataDeviceAddress.deviceAddress = getBufferDeviceAddress(logicalDevice, uboInstance.getBuffer());

	VkAccelerationStructureGeometryKHR accelerationStructureGeometry = {};
	accelerationStructureGeometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	accelerationStructureGeometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	accelerationStructureGeometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
	accelerationStructureGeometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	accelerationStructureGeometry.geometry.instances.arrayOfPointers = VK_FALSE;
	accelerationStructureGeometry.geometry.instances.data = instanceDataDeviceAddress;

	VkAccelerationStructureBuildGeometryInfoKHR accelerationStructureBuildGeometryInfo = {};
	accelerationStructureBuildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	accelerationStructureBuildGeometryInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
	accelerationStructureBuildGeometryInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
	accelerationStructureBuildGeometryInfo.mode = VK_NULL_HANDLE == tlas.handle ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
	accelerationStructureBuildGeometryInfo.geometryCount = 1;
	accelerationStructureBuildGeometryInfo.pGeometries = &accelerationStructureGeometry;
	accelerationStructureBuildGeometryInfo.srcAccelerationStructure = VK_NULL_HANDLE != tlas.handle ? tlas.handle : VK_NULL_HANDLE;

	uint32_t primitiveCount = static_cast<uint32_t>(instances.size());

	VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfo = {};
	accelerationStructureBuildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	vkGetAccelerationStructureBuildSizesKHR(logicalDevice.getDevice(),
		VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
		&accelerationStructureBuildGeometryInfo,
		&primitiveCount,
		&accelerationStructureBuildSizesInfo);

	if (VK_NULL_HANDLE == tlas.handle)
	{
		tlas.createAccelerationStructureBuffer(logicalDevice, accelerationStructureBuildSizesInfo);
	}

	RayTracingScratchBuffer scratchBuffer = createScratchBuffer(logicalDevice, accelerationStructureBuildSizesInfo.buildScratchSize);

	accelerationStructureBuildGeometryInfo.dstAccelerationStructure = tlas.handle;
	accelerationStructureBuildGeometryInfo.scratchData.deviceAddress = scratchBuffer.deviceAddress;

	VkAccelerationStructureBuildRangeInfoKHR accelerationStructureBuildRangeInfo = {};
	accelerationStructureBuildRangeInfo.primitiveCount = primitiveCount;
	accelerationStructureBuildRangeInfo.primitiveOffset = 0;
	accelerationStructureBuildRangeInfo.firstVertex = 0;
	accelerationStructureBuildRangeInfo.transformOffset = 0;

	std::vector<VkAccelerationStructureBuildRangeInfoKHR*> list = { &accelerationStructureBuildRangeInfo };

	buildAccelerationStructure(logicalDevice, commandPool, accelerationStructureBuildGeometryInfo, list);

	scratchBuffer.destroy(logicalDevice);
	uboInstance.destroy(logicalDevice);
}

void RayTracer::destroyShaderBindingTable(LogicalDevice& logicalDevice)
{
	uboSBTClosestHit.destroy(logicalDevice);
	uboSBTMiss.destroy(logicalDevice);
	uboSBTRaygen.destroy(logicalDevice);
}

void RayTracer::destroyStorageImage(LogicalDevice& logicalDevice)
{
	storageImage.destroy(logicalDevice);
	outputImage.destroy(logicalDevice);
}

VkDeviceAddress RayTracer::getBufferDeviceAddress(LogicalDevice& logicalDevice, VkBuffer buffer)
{
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR =
		reinterpret_cast<PFN_vkGetBufferDeviceAddressKHR>(vkGetDeviceProcAddr(logicalDevice.getDevice(), "vkGetBufferDeviceAddressKHR"));
	VkBufferDeviceAddressInfoKHR bufferDeviceAI = {};

	bufferDeviceAI.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	bufferDeviceAI.buffer = buffer;

	return vkGetBufferDeviceAddressKHR(logicalDevice.getDevice(), &bufferDeviceAI);
}