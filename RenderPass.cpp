#include "RenderPass.h"

#include <vector>

RenderPass::RenderPass()
{
    renderPass = VK_NULL_HANDLE;
}

RenderPass::~RenderPass() {}

void RenderPass::beginRenderPass(uint32_t height, uint32_t width, VkFramebuffer framebuffer, VkCommandBuffer commandBuffer)
{
	VkRenderPassBeginInfo renderPassBeginInfo{};
	VkRect2D renderArea{};
	std::vector<VkClearValue> clearValues;
	VkExtent2D extent = {width, height};
	VkViewport viewport{};
	VkRect2D scissor{};

	renderArea.offset = {0, 0};
	renderArea.extent = extent;

	clearValues.resize(2);
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.clearValueCount = clearValues.size();
	renderPassBeginInfo.pClearValues = clearValues.data();
	renderPassBeginInfo.framebuffer = framebuffer;

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = extent.width;
	viewport.height = extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.offset = { 0, 0 };
	scissor.extent = extent;

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void RenderPass::destroy(LogicalDevice& logicalDevice) { vkDestroyRenderPass(logicalDevice.getDevice(), renderPass, nullptr); }
VkRenderPass RenderPass::getRenderPass() { return renderPass; }