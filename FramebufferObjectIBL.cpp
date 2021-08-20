#include "FramebufferObjectIBL.h"

//public
FramebufferObjectIBL::FramebufferObjectIBL(VkFormat colorFormat, uint32_t height, uint32_t width)
{
	this->colorFormat = colorFormat;
	this->height = height;
	this->width = width;
}

FramebufferObjectIBL::~FramebufferObjectIBL() {}

void FramebufferObjectIBL::create(const LogicalDevice& logicalDevice, RenderPass* renderPass)
{
	colorAttachment.create(logicalDevice,
		height,
		width,
		colorFormat,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		false,
		VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE);

	framebuffer.create(logicalDevice, renderPass->getRenderPass(), height, width, colorAttachment.getImageView());
}