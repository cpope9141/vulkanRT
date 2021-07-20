#include "Image.h"

#include "CommandBuffer.h"
#include "MemoryUtilities.h"

#include <cmath>
#include <iostream>

const uint8_t BYTES_8_BITS_PER_CHANNEL = 4;
const uint8_t BYTES_32_BITS_PER_CHANNEL = 16;

//public
Image::Image()
{
	arrayLayers = 1;
	format = VK_FORMAT_UNDEFINED;
	image = VK_NULL_HANDLE;
	imageMemory = VK_NULL_HANDLE;
	mipLevels = 1;
}

Image::~Image() {}

void Image::copyToImage(VkImage dst, uint32_t height, uint32_t width, uint32_t baseArrayLayer, uint32_t mipLevel, CommandBuffer commandBuffer)
{
	VkImageCopy copyRegion = {};

	copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.srcSubresource.baseArrayLayer = 0;
	copyRegion.srcSubresource.mipLevel = 0;
	copyRegion.srcSubresource.layerCount = 1;

	copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.dstSubresource.baseArrayLayer = baseArrayLayer;
	copyRegion.dstSubresource.mipLevel = mipLevel;
	copyRegion.dstSubresource.layerCount = 1;

	copyRegion.extent = { width, height, 1 };

	vkCmdCopyImage(commandBuffer.getCommandBuffer(),
		image,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		dst,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&copyRegion);
}

void Image::create(LogicalDevice logicalDevice,
	uint32_t height,
	uint32_t width,
	VkFormat format,
	VkImageUsageFlags usage,
	VkSampleCountFlagBits samples,
	uint32_t arrayLayers,
	VkImageCreateFlags flags,
	bool genMipLevels)
{
	this->arrayLayers = arrayLayers;
	this->format = format;

	if (genMipLevels)
	{
		mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
	}

	createImage(logicalDevice,
		width,
		height,
		VK_IMAGE_TILING_OPTIMAL,
		usage,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		samples,
		flags);
}

void Image::create(LogicalDevice logicalDevice, CommandPool commandPool, const RGBAResource& resource)
{
	VkDeviceSize imageSize = static_cast<VkDeviceSize>(resource.width) * resource.height * BYTES_8_BITS_PER_CHANNEL;
	VkBuffer pStagingBuffer;
	VkDeviceMemory pStagingBufferMemory;
	void* data = nullptr;
	CommandBuffer commandBuffer = CommandBuffer(commandPool);

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(resource.width, resource.height)))) + 1;

	createBuffer(logicalDevice,
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&pStagingBuffer,
		&pStagingBufferMemory);

	vkMapMemory(logicalDevice.getDevice(), pStagingBufferMemory, 0, imageSize, 0, &data);
	std::memcpy(data, resource.imageRGBA, imageSize);
	vkUnmapMemory(logicalDevice.getDevice(), pStagingBufferMemory);

	create(logicalDevice,
		resource.height,
		resource.width,
		VK_FORMAT_R8G8B8A8_UNORM,
		(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		VK_SAMPLE_COUNT_1_BIT,
		1,
		0,
		false);

	transitionLayout(logicalDevice,
		commandBuffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		true);

	copyBufferToImage(logicalDevice, commandPool, pStagingBuffer, image, resource.width, resource.height, 1, imageSize);

	generateMipmaps(logicalDevice, commandPool, VK_FORMAT_R8G8B8A8_SRGB, resource.width, resource.height, mipLevels);

	vkDestroyBuffer(logicalDevice.getDevice(), pStagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getDevice(), pStagingBufferMemory, nullptr);
}

void Image::create(LogicalDevice& logicalDevice, CommandPool& commandPool, HDRResource resources[6])
{
	VkDeviceSize imageSize = resources[0].width * resources[0].height * 6 * BYTES_32_BITS_PER_CHANNEL;
	VkDeviceSize bytes = resources[0].width * resources[0].height * BYTES_32_BITS_PER_CHANNEL;
	VkBuffer pStagingBuffer;
	VkDeviceMemory pStagingBufferMemory;
	void* data = nullptr;
	uint8_t* dst = nullptr;
	CommandBuffer commandBuffer(commandPool);

	createBuffer(logicalDevice,
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&pStagingBuffer,
		&pStagingBufferMemory);

	vkMapMemory(logicalDevice.getDevice(), pStagingBufferMemory, 0, imageSize, 0, &data);

	dst = reinterpret_cast<uint8_t*>(data);
	for (uint8_t i = 0; i < 6; i++)
	{
		std::memcpy(dst + i * bytes, resources[i].imageHDR, bytes);
	}

	vkUnmapMemory(logicalDevice.getDevice(), pStagingBufferMemory);

	create(logicalDevice,
		resources[0].width,
		resources[0].height,
		VK_FORMAT_R32G32B32A32_SFLOAT,
		(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		VK_SAMPLE_COUNT_1_BIT,
		6,
		VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
		false);

	transitionLayout(logicalDevice,
		commandBuffer,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		true);

	copyBufferToImage(logicalDevice, commandPool, pStagingBuffer, image, resources[0].width, resources[0].height, 6, bytes);

	transitionLayout(logicalDevice,
		commandBuffer,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		true);

	vkDestroyBuffer(logicalDevice.getDevice(), pStagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getDevice(), pStagingBufferMemory, nullptr);
}

void Image::destroy(LogicalDevice logicalDevice)
{
	vkDestroyImage(logicalDevice.getDevice(), image, nullptr);
	vkFreeMemory(logicalDevice.getDevice(), imageMemory, nullptr);
}

VkImage Image::getImage() { return image; }
uint32_t Image::getMipLevels() { return mipLevels; }

void Image::transitionLayout(LogicalDevice logicalDevice, CommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout, bool submit)
{
	VkImageMemoryBarrier barrier = {};
	VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
	VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;

	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = arrayLayers;

	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	if (VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL == newLayout) {
		int aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (hasStencilComponent(format)) { aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT; }

		barrier.subresourceRange.aspectMask = aspectMask;
	}

	switch (oldLayout)
	{
		case VK_IMAGE_LAYOUT_GENERAL:
			barrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_UNDEFINED:
			barrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		default:
			throw std::runtime_error("Unsupported oldLayout transition");
	}

	switch (newLayout)
	{
		case VK_IMAGE_LAYOUT_GENERAL:
			barrier.dstAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			barrier.dstAccessMask = (VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (0 == barrier.srcAccessMask) { barrier.srcAccessMask = (VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT); }
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		default:
			throw std::runtime_error("Unsupported newLayout transition");
	}

	if (submit)
	{
		commandBuffer.beginOneTimeCommandBuffer(logicalDevice);
		vkCmdPipelineBarrier(commandBuffer.getCommandBuffer(), srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
	}
	else
	{
		vkCmdPipelineBarrier(commandBuffer.getCommandBuffer(), srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}
}

//private
void Image::copyBufferToImage(LogicalDevice logicalDevice, CommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, size_t regionCount, VkDeviceSize bytes)
{
	CommandBuffer commandBuffer = CommandBuffer(commandPool);
	std::vector<VkBufferImageCopy> regions;
	regions.resize(regionCount);

	for (uint8_t i = 0; i < regions.capacity(); i++) {
		regions[i].bufferOffset = i * bytes;
		regions[i].bufferRowLength = 0;
		regions[i].bufferImageHeight = 0;
		regions[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		regions[i].imageSubresource.mipLevel = 0;
		regions[i].imageSubresource.baseArrayLayer = i;
		regions[i].imageSubresource.layerCount = 1;
		//regions[i].imageOffset.set(0, 0, 0);
		regions[i].imageOffset = { 0, 0, 0 };
		//regions[i].imageExtent(VkExtent3D.callocStack(stack).set(width, height, 1));
		regions[i].imageExtent = { width, height, 1 };
	}

	commandBuffer.beginOneTimeCommandBuffer(logicalDevice);
	vkCmdCopyBufferToImage(commandBuffer.getCommandBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions.size(), regions.data());
	commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
}

void Image::createImage(LogicalDevice logicalDevice,
	uint32_t width,
	uint32_t height,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags memProperties,
	VkSampleCountFlagBits samples,
	VkImageCreateFlags flags)
{
	VkImageCreateInfo imageCreateInfo = {};

	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = mipLevels;
	imageCreateInfo.arrayLayers = arrayLayers;
	imageCreateInfo.format = format;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = usage;
	imageCreateInfo.samples = samples;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.flags = flags;

	if (VK_SUCCESS == vkCreateImage(logicalDevice.getDevice(), &imageCreateInfo, nullptr, &image))
	{
		VkMemoryRequirements memRequirements = {};
		VkMemoryAllocateInfo allocInfo = {};
		vkGetImageMemoryRequirements(logicalDevice.getDevice(), image, &memRequirements);

		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(logicalDevice, memRequirements.memoryTypeBits, memProperties);

		if (VK_SUCCESS == vkAllocateMemory(logicalDevice.getDevice(), &allocInfo, nullptr, &imageMemory))
		{
			if (VK_SUCCESS != vkBindImageMemory(logicalDevice.getDevice(), image, imageMemory, 0))
			{
				throw std::runtime_error("Failed to bind image memory");
			}
		}
		else
		{
			throw std::runtime_error("Failed to allocate image memory");
		}
	}
	else
	{
		throw std::runtime_error("Failed to create image");
	}
}

void Image::generateMipmaps(LogicalDevice logicalDevice, CommandPool commandPool, VkFormat imageFormat, int width, int height, uint32_t mipLevels)
{
	VkFormatProperties formatProperties = {};
	vkGetPhysicalDeviceFormatProperties(logicalDevice.getPhysicalDevicePtr()->getPhysicalDevice(), imageFormat, &formatProperties);

	if (0 != (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
	{
		int mipWidth = width;
		int mipHeight = height;
		CommandBuffer commandBuffer = CommandBuffer(commandPool);

		commandBuffer.beginOneTimeCommandBuffer(logicalDevice);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstAccessMask = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		for (int i = 1; i < mipLevels; i++)
		{
			barrier.subresourceRange.baseMipLevel = (i - 1);
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer.getCommandBuffer(),
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&barrier);

			VkImageBlit blit = {};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = (i - 1);
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer.getCommandBuffer(),
				image,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer.getCommandBuffer(),
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&barrier);

			if (mipWidth > 1) { mipWidth /= 2; }
			if (mipHeight > 1) { mipHeight /= 2; }
		}

		barrier.subresourceRange.baseMipLevel = (mipLevels - 1);
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer.getCommandBuffer(),
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0,
			0,
			nullptr,
			0,
			nullptr,
			1,
			&barrier);

		commandBuffer.submitOneTimeCommandBuffer(logicalDevice);
	}
	else
	{
		throw std::runtime_error("Image format does not support linear blitting");
	}
}

bool Image::hasStencilComponent(VkFormat format) { return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT; }