#include "DescriptorSetIrradiance.h"

//public
DescriptorSetIrradiance::DescriptorSetIrradiance()
{
	cubeMap = nullptr;
}

DescriptorSetIrradiance::~DescriptorSetIrradiance() {}

void DescriptorSetIrradiance::create(LogicalDevice logicalDevice, GraphicsPipelineIrradiance* gp, CubeMap* cubeMap)
{
    this->cubeMap = cubeMap;

	allocateDescriptorSet(logicalDevice, gp->getDescriptorSetLayout(), gp->getDescriptorPool());
}

//protected
void DescriptorSetIrradiance::updateDescriptorSet(LogicalDevice logicalDevice)
{
    VkDescriptorImageInfo irradianceImageInfo = createDescriptorImageInfo(cubeMap, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
    writeDescriptorSets.resize(1);

    writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[0].dstBinding = 0;
    writeDescriptorSets[0].dstArrayElement = 0;
    writeDescriptorSets[0].dstSet = descriptorSet;
    writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[0].descriptorCount = 1;
    writeDescriptorSets[0].pImageInfo = &irradianceImageInfo;

    vkUpdateDescriptorSets(logicalDevice.getDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
}