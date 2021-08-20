#include "DescriptorSetPBR.h"

#include "GraphicsPipeline.h"

//public
DescriptorSetPBR::DescriptorSetPBR()
{
	albedo = nullptr;
	ao = nullptr;
    integratedBRDF = nullptr;
    irradiance = nullptr;
	metallic = nullptr;
	normal = nullptr;
    prefiltered = nullptr;
	roughness = nullptr;
    uboLighting = nullptr;
	uboProjection = nullptr;
    uboStaticModel = nullptr;
}

DescriptorSetPBR::~DescriptorSetPBR() {}

void DescriptorSetPBR::create(LogicalDevice& logicalDevice,
	GraphicsPipeline* gp,
	UniformBufferProjection* uboProjection,
	UniformBufferPBRLighting* uboLighting,
	UniformBufferStaticModelPBR* uboStaticModel,
	PrecomputedIBL* precomputedIBL,
	ModelRT* modelRT)
{
	albedo = modelRT->getAlbedoPtr();
	ao = modelRT->getAmbientOcclusionPtr();
	integratedBRDF = precomputedIBL->getIntegratedBRDF();
	irradiance = precomputedIBL->getIrradianceCubeMap();
	metallic = modelRT->getMetallicPtr();
	normal = modelRT->getNormalPtr();
	prefiltered = precomputedIBL->getPrefilteredCubeMap();
	roughness = modelRT->getRoughnessPtr();
	this->uboLighting = uboLighting;
	this->uboProjection = uboProjection;
	this->uboStaticModel = uboStaticModel;

	allocateDescriptorSet(logicalDevice, gp->getDescriptorSetLayout(), gp->getDescriptorPool());
}

void DescriptorSetPBR::destroy(LogicalDevice& logicalDevice, GraphicsPipeline* gp)
{
    vkFreeDescriptorSets(logicalDevice.getDevice(), gp->getDescriptorPool(), 1, &descriptorSet);
}

//protected
void DescriptorSetPBR::updateDescriptorSet(const LogicalDevice& logicalDevice)
{
    VkDescriptorBufferInfo dbiProjection = createDescriptorBufferInfo(uboProjection, 0);
    VkDescriptorBufferInfo dbiLighting = createDescriptorBufferInfo(uboLighting, 0);
    VkDescriptorBufferInfo dbiStaticModel = createDescriptorBufferInfo(uboStaticModel, 0);

    VkDescriptorImageInfo albedoImageInfo = createDescriptorImageInfo(albedo, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo aoImageInfo = createDescriptorImageInfo(ao, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo integratedBRDFImageInfo = createDescriptorImageInfo(integratedBRDF, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo irradianceImageInfo = createDescriptorImageInfo(irradiance, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo metallicImageInfo = createDescriptorImageInfo(metallic, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo normalImageInfo = createDescriptorImageInfo(normal, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo prefilteredImageInfo = createDescriptorImageInfo(prefiltered, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    VkDescriptorImageInfo roughnessImageInfo = createDescriptorImageInfo(roughness, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
    writeDescriptorSets.resize(11);

    writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[0].dstBinding = 0;
    writeDescriptorSets[0].dstArrayElement = 0;
    writeDescriptorSets[0].dstSet = descriptorSet;
    writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSets[0].descriptorCount = 1;
    writeDescriptorSets[0].pBufferInfo = &dbiProjection;

    writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[1].dstBinding = 1;
    writeDescriptorSets[1].dstArrayElement = 0;
    writeDescriptorSets[1].dstSet = descriptorSet;
    writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSets[1].descriptorCount = 1;
    writeDescriptorSets[1].pBufferInfo = &dbiLighting;

    writeDescriptorSets[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[2].dstBinding = 2;
    writeDescriptorSets[2].dstArrayElement = 0;
    writeDescriptorSets[2].dstSet = descriptorSet;
    writeDescriptorSets[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSets[2].descriptorCount = 1;
    writeDescriptorSets[2].pBufferInfo = &dbiStaticModel;

    writeDescriptorSets[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[3].dstBinding = 3;
    writeDescriptorSets[3].dstArrayElement = 0;
    writeDescriptorSets[3].dstSet = descriptorSet;
    writeDescriptorSets[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[3].descriptorCount = 1;
    writeDescriptorSets[3].pImageInfo = &albedoImageInfo;

    writeDescriptorSets[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[4].dstBinding = 4;
    writeDescriptorSets[4].dstArrayElement = 0;
    writeDescriptorSets[4].dstSet = descriptorSet;
    writeDescriptorSets[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[4].descriptorCount = 1;
    writeDescriptorSets[4].pImageInfo = &aoImageInfo;

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
    writeDescriptorSets[7].pImageInfo = &metallicImageInfo;

    writeDescriptorSets[8].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[8].dstBinding = 8;
    writeDescriptorSets[8].dstArrayElement = 0;
    writeDescriptorSets[8].dstSet = descriptorSet;
    writeDescriptorSets[8].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[8].descriptorCount = 1;
    writeDescriptorSets[8].pImageInfo = &normalImageInfo;

    writeDescriptorSets[9].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[9].dstBinding = 9;
    writeDescriptorSets[9].dstArrayElement = 0;
    writeDescriptorSets[9].dstSet = descriptorSet;
    writeDescriptorSets[9].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[9].descriptorCount = 1;
    writeDescriptorSets[9].pImageInfo = &prefilteredImageInfo;

    writeDescriptorSets[10].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[10].dstBinding = 10;
    writeDescriptorSets[10].dstArrayElement = 0;
    writeDescriptorSets[10].dstSet = descriptorSet;
    writeDescriptorSets[10].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[10].descriptorCount = 1;
    writeDescriptorSets[10].pImageInfo = &roughnessImageInfo;

    vkUpdateDescriptorSets(logicalDevice.getDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
}