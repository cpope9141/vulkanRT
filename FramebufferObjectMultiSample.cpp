#include "FramebufferObjectMultiSample.h"

#include <iostream>

//public
FramebufferObjectMultiSample::FramebufferObjectMultiSample() {}
FramebufferObjectMultiSample::FramebufferObjectMultiSample(uint32_t height, uint32_t width, VkFormat colorFormat) : FramebufferObject(height, width, colorFormat) {}
FramebufferObjectMultiSample::~FramebufferObjectMultiSample() {}

void FramebufferObjectMultiSample::create(LogicalDevice& logicalDevice, RenderPass* renderPass)
{
	VkSampleCountFlagBits sampleCount = logicalDevice.getPhysicalDevicePtr()->getSampleCount();

	colorAttachment.create(logicalDevice,
		height,
		width,
		colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,
		(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		VK_SAMPLE_COUNT_1_BIT,
		true,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK);

	depthAttachment.create(logicalDevice,
		height,
		width,
		logicalDevice.getPhysicalDevicePtr()->getDepthFormat(),
		VK_IMAGE_ASPECT_DEPTH_BIT,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		sampleCount,
		VK_TRUE,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK);

	msImage.create(logicalDevice,
		height,
		width,
		colorFormat,
		VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		sampleCount,
		1,
		0,
		VK_FALSE);

	msImageView.create(logicalDevice, msImage.getImage(), VK_IMAGE_VIEW_TYPE_2D, 1, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	framebuffer.create(logicalDevice,
		renderPass->getRenderPass(),
		height,
		width,
		msImageView.getImageView(),
		depthAttachment.getImageView(),
		colorAttachment.getImageView());
}

void FramebufferObjectMultiSample::destroy(LogicalDevice& logicalDevice) {
	FramebufferObject::destroy(logicalDevice);
	msImageView.destroy(logicalDevice);
	msImage.destroy(logicalDevice);
}