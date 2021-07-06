#pragma once
#include "LogicalDevice.h"
#include "Texture.h"
#include "UniformBufferObject.h"
#include "vulkan/vulkan_core.h"

#include <vector>

class DescriptorSet
{
public:
	DescriptorSet();
	~DescriptorSet();

	void createDescriptorSets(LogicalDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout, VkDescriptorPool descriptorPool);

protected:
	VkDescriptorSet descriptorSet;

	VkDescriptorBufferInfo createDescriptorBufferInfo(UniformBufferObject* ubo, VkDeviceSize offset);
	VkDescriptorImageInfo createDescriptorImageInfo(Texture* texture, VkImageLayout imageLayout);
	virtual std::vector<VkWriteDescriptorSet> writeDescriptorSets(LogicalDevice logicalDevice) = 0;
};