#include "DescriptorSet.h"

#include <iostream>

//public
DescriptorSet::DescriptorSet()
{
	descriptorSet = VK_NULL_HANDLE;
}

DescriptorSet::~DescriptorSet() {}

void DescriptorSet::createDescriptorSets(LogicalDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool)
{
    VkDescriptorSetAllocateInfo allocInfo = {};

    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    if (VK_SUCCESS == vkAllocateDescriptorSets(logicalDevice.getDevice(), &allocInfo, &descriptorSet))
    {
        std::vector<VkWriteDescriptorSet> writeDescriptorSet = writeDescriptorSets(logicalDevice);

        vkUpdateDescriptorSets(logicalDevice.getDevice(), writeDescriptorSet.size(), writeDescriptorSet.data(), 0, nullptr);
    }
    else {
        throw std::runtime_error("Failed to allocate descriptor sets");
    }
}

//protected
VkDescriptorBufferInfo DescriptorSet::createDescriptorBufferInfo(UniformBufferObject* ubo, VkDeviceSize offset)
{
    VkDescriptorBufferInfo dbi = {};

    dbi.offset = offset;
    dbi.range = ubo->getSize();
    dbi.buffer = ubo->getBuffer();

    return dbi;
}

VkDescriptorImageInfo DescriptorSet::createDescriptorImageInfo(Texture* texture, VkImageLayout imageLayout)
{
    VkDescriptorImageInfo dii = {};

    dii.imageLayout = imageLayout;
    dii.imageView = texture->getImageView();
    dii.sampler = texture->getSampler();

    return dii;
}