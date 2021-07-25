#include "DescriptorSetRTPBR.h"

#include "PrecomputedIBL.h"
#include "RayTracingPipelinePBR.h"

//public
DescriptorSetRTPBR::DescriptorSetRTPBR()
{
}

DescriptorSetRTPBR::~DescriptorSetRTPBR()
{
}

void DescriptorSetRTPBR::create(LogicalDevice& logicalDevice,
	RayTracingPipelinePBR* rtp,
	Texture* storageImage,
	AccelerationStructure* tlas,
	UniformBufferRT* uboRT,
	SceneStorageBuffer* sceneStorageBuffer,
	std::vector<Texture*>& textures,
	PrecomputedIBL* precomputedIBL,
	CubeMap* environment)
{
	this->sceneStorageBuffer = sceneStorageBuffer;
	this->storageImage = storageImage;
	this->tlas = tlas;
	this->uboRT = uboRT;
    this->textures.clear();
    this->textures.resize(textures.size());
	for (size_t i = 0; i < textures.size(); i++)
	{
		this->textures[i] = textures[i];
	}

	integratedBRDF = precomputedIBL->getIntegratedBRDF();
	irradiance = precomputedIBL->getIrradianceCubeMap();
	prefiltered = precomputedIBL->getPrefilteredCubeMap();

	this->environment = environment;

	allocateDescriptorSet(logicalDevice, rtp->getDescriptorSetLayout(), rtp->getDescriptorPool());
}

//protected
void DescriptorSetRTPBR::updateDescriptorSet(LogicalDevice& logicalDevice)
{
    VkWriteDescriptorSetAccelerationStructureKHR descriptorAccelerationStructureInfo = {};

    VkDescriptorBufferInfo descriptorBufferInfo = createDescriptorBufferInfo(uboRT, 0);
    VkDescriptorImageInfo storageImageDescriptor = createDescriptorImageInfo(storageImage, VK_IMAGE_LAYOUT_GENERAL);
    VkDescriptorBufferInfo sceneStorageDescriptor;// = createDescriptorBufferInfo(sceneStorageBuffer, 0);
    sceneStorageDescriptor.offset = 0;
    sceneStorageDescriptor.buffer = sceneStorageBuffer->getBuffer();
    sceneStorageDescriptor.range = VK_WHOLE_SIZE;
    std::vector<VkDescriptorImageInfo> texturesdiis;
    texturesdiis.resize(textures.size());
    VkDescriptorImageInfo integratedBRDFImageInfo = createDescriptorImageInfo(integratedBRDF, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo irradianceImageInfo = createDescriptorImageInfo(irradiance, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo prefilteredImageInfo = createDescriptorImageInfo(prefiltered, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo environmentImageInfo = createDescriptorImageInfo(environment, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
    writeDescriptorSets.resize(9);

    descriptorAccelerationStructureInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    descriptorAccelerationStructureInfo.accelerationStructureCount = 1;
    descriptorAccelerationStructureInfo.pAccelerationStructures = &tlas->handle;

    writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[0].dstBinding = 0;
    writeDescriptorSets[0].dstSet = descriptorSet;
    writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    writeDescriptorSets[0].descriptorCount = 1;
    // The specialized acceleration structure descriptor has to be chained
    writeDescriptorSets[0].pNext = &descriptorAccelerationStructureInfo;

    writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[1].dstBinding = 1;
    writeDescriptorSets[1].dstArrayElement = 0;
    writeDescriptorSets[1].dstSet = descriptorSet;
    writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    writeDescriptorSets[1].descriptorCount = 1;
    writeDescriptorSets[1].pImageInfo = &storageImageDescriptor;

    writeDescriptorSets[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[2].dstBinding = 2;
    writeDescriptorSets[2].dstArrayElement = 0;
    writeDescriptorSets[2].dstSet = descriptorSet;
    writeDescriptorSets[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSets[2].descriptorCount = 1;
    writeDescriptorSets[2].pBufferInfo = &descriptorBufferInfo;

    writeDescriptorSets[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[3].dstBinding = 3;
    writeDescriptorSets[3].dstArrayElement = 0;
    writeDescriptorSets[3].dstSet = descriptorSet;
    writeDescriptorSets[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writeDescriptorSets[3].descriptorCount = 1;
    writeDescriptorSets[3].pBufferInfo = &sceneStorageDescriptor;

    for (size_t i = 0; i < textures.size(); i++)
    {
        texturesdiis[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        texturesdiis[i].imageView = textures[i]->getImageView();
        texturesdiis[i].sampler = textures[i]->getSampler();
    }

    writeDescriptorSets[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[4].dstBinding = 4;
    writeDescriptorSets[4].dstArrayElement = 0;
    writeDescriptorSets[4].dstSet = descriptorSet;
    writeDescriptorSets[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[4].descriptorCount = static_cast<uint32_t>(texturesdiis.size());
    writeDescriptorSets[4].pImageInfo = texturesdiis.data();

    writeDescriptorSets[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[5].dstBinding = 5;
    writeDescriptorSets[5].dstArrayElement = 0;
    writeDescriptorSets[5].dstSet = descriptorSet;
    writeDescriptorSets[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[5].descriptorCount = 1;
    writeDescriptorSets[5].pImageInfo = &integratedBRDFImageInfo;

    writeDescriptorSets[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[6].dstBinding = 6;
    writeDescriptorSets[6].dstArrayElement = 0;
    writeDescriptorSets[6].dstSet = descriptorSet;
    writeDescriptorSets[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[6].descriptorCount = 1;
    writeDescriptorSets[6].pImageInfo = &irradianceImageInfo;

    writeDescriptorSets[7].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[7].dstBinding = 7;
    writeDescriptorSets[7].dstArrayElement = 0;
    writeDescriptorSets[7].dstSet = descriptorSet;
    writeDescriptorSets[7].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[7].descriptorCount = 1;
    writeDescriptorSets[7].pImageInfo = &prefilteredImageInfo;

    writeDescriptorSets[8].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[8].dstBinding = 8;
    writeDescriptorSets[8].dstArrayElement = 0;
    writeDescriptorSets[8].dstSet = descriptorSet;
    writeDescriptorSets[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[8].descriptorCount = 1;
    writeDescriptorSets[8].pImageInfo = &environmentImageInfo;

    vkUpdateDescriptorSets(logicalDevice.getDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
}