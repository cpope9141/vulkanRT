#include "DescriptorSetIntegratedBRDF.h"

//public
DescriptorSetIntegratedBRDF::DescriptorSetIntegratedBRDF()
{
    ubo = nullptr;
}

DescriptorSetIntegratedBRDF::~DescriptorSetIntegratedBRDF() {}

void DescriptorSetIntegratedBRDF::create(const LogicalDevice& logicalDevice, GraphicsPipelineIntegratedBRDF* gp, UniformBufferProjection* ubo)
{
    this->ubo = ubo;

    allocateDescriptorSet(logicalDevice, gp->getDescriptorSetLayout(), gp->getDescriptorPool());
}

//protected
void DescriptorSetIntegratedBRDF::updateDescriptorSet(const LogicalDevice& logicalDevice)
{
    VkDescriptorBufferInfo descriptorBufferInfo = createDescriptorBufferInfo(ubo, 0);

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
    writeDescriptorSets.resize(1);

    writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[0].dstBinding = 0;
    writeDescriptorSets[0].dstArrayElement = 0;
    writeDescriptorSets[0].dstSet = descriptorSet;
    writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSets[0].descriptorCount = 1;
    writeDescriptorSets[0].pBufferInfo = &descriptorBufferInfo;

    vkUpdateDescriptorSets(logicalDevice.getDevice(), writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
}