#include "FramebufferObject.h"

//public
FramebufferObject::FramebufferObject()
{
	colorFormat = VK_FORMAT_UNDEFINED;
	height = 0;
	width = 0;
}

FramebufferObject::FramebufferObject(uint32_t height, uint32_t width, VkFormat colorFormat)
{
	this->colorFormat = colorFormat;
	this->height = height;
	this->width = width;
}

FramebufferObject::~FramebufferObject() {}

void FramebufferObject::create(LogicalDevice logicalDevice, RenderPass* renderPass)
{
	colorAttachment.create(logicalDevice,
		height,
		width,
		colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,
		(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
		VK_SAMPLE_COUNT_1_BIT,
		VK_TRUE,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK);

	depthAttachment.create(logicalDevice,
		height,
		width,
		logicalDevice.getPhysicalDevicePtr()->getDepthFormat(),
		VK_IMAGE_ASPECT_DEPTH_BIT,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		VK_TRUE,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK);

	framebuffer.create(logicalDevice, renderPass->getRenderPass(), height, width, colorAttachment.getImageView(), depthAttachment.getImageView(), VK_NULL_HANDLE);
}

void FramebufferObject::destroy(LogicalDevice logicalDevice)
{
	framebuffer.destroy(logicalDevice);
	depthAttachment.destroy(logicalDevice);
	colorAttachment.destroy(logicalDevice);
}

Texture FramebufferObject::getColorAttachment() { return colorAttachment; }
Texture FramebufferObject::getDepthAttachment() { return depthAttachment; }
VkFramebuffer FramebufferObject::getFramebuffer() { return framebuffer.getFramebuffer(); }
uint32_t FramebufferObject::getHeight() { return height; }
uint32_t FramebufferObject::getWidth() { return width; }