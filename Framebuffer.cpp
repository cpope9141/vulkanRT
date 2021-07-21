#include "Framebuffer.h"

#include <iostream>

const int ATTACHMENT_COUNT_SINGLE_SAMPLE = 2;
const int ATTACHMENT_COUNT_MULTI_SAMPLE = 3;

//public
Framebuffer::Framebuffer()
{
    framebuffer = VK_NULL_HANDLE;
}

Framebuffer::~Framebuffer() {}

void Framebuffer::create(LogicalDevice& logicalDevice, VkRenderPass renderPass, uint32_t height, uint32_t width, VkImageView imageView)
{
    VkFramebufferCreateInfo framebufferInfo = {};

    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &imageView;

    if (VK_SUCCESS != vkCreateFramebuffer(logicalDevice.getDevice(), &framebufferInfo, nullptr, &framebuffer))
    {
        throw std::runtime_error("Failed to create framebuffer");
    }
}

void Framebuffer::create(LogicalDevice& logicalDevice, VkRenderPass renderPass, uint32_t height, uint32_t width, VkImageView imageView, VkImageView depthView, VkImageView resolveView)
{
    int attachmentCount = (VK_NULL_HANDLE == resolveView ? ATTACHMENT_COUNT_SINGLE_SAMPLE : ATTACHMENT_COUNT_MULTI_SAMPLE);
    std::vector<VkImageView> attachments(attachmentCount, VK_NULL_HANDLE);
    VkFramebufferCreateInfo framebufferInfo = {};

    attachments[0] = imageView;
    attachments[1] = depthView;

    if (ATTACHMENT_COUNT_MULTI_SAMPLE == attachmentCount) {
        attachments[ATTACHMENT_COUNT_MULTI_SAMPLE - 1] = resolveView;
    }

    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;
    framebufferInfo.attachmentCount = attachmentCount;
    framebufferInfo.pAttachments = attachments.data();

    if (VK_SUCCESS != vkCreateFramebuffer(logicalDevice.getDevice(), &framebufferInfo, nullptr, &framebuffer))
    {
        throw std::runtime_error("Failed to create framebuffer");
    }
}

void Framebuffer::destroy(LogicalDevice& logicalDevice) { vkDestroyFramebuffer(logicalDevice.getDevice(), framebuffer, nullptr); }
VkFramebuffer Framebuffer::getFramebuffer() { return framebuffer; }