#include "DescriptorSetPostProcess.h"

//public
DescriptorSetPostProcess::DescriptorSetPostProcess() {}
DescriptorSetPostProcess::~DescriptorSetPostProcess() {}

//protected
std::vector<VkWriteDescriptorSet> DescriptorSetPostProcess::writeDescriptorSets(LogicalDevice logicalDevice)
{
    VkDescriptorBufferInfo descriptorBufferInfo = createDescriptorBufferInfo(ubo, 0);
    VkDescriptorImageInfo descriptorImageInfo = createDescriptorImageInfo(texture, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
    writeDescriptorSets.resize(2);

    writeDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[0].dstBinding = 0;
    writeDescriptorSets[0].dstArrayElement = 0;
    writeDescriptorSets[0].dstSet = descriptorSet;
    writeDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writeDescriptorSets[0].descriptorCount = 1;
    writeDescriptorSets[0].pBufferInfo = &descriptorBufferInfo;

    writeDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[1].dstBinding = 1;
    writeDescriptorSets[1].dstArrayElement = 0;
    writeDescriptorSets[1].dstSet = descriptorSet;
    writeDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[1].descriptorCount = 1;
    writeDescriptorSets[1].pImageInfo = &descriptorImageInfo;

    return writeDescriptorSets;
}