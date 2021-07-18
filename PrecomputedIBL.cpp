#include "PrecomputedIBL.h"

#include "DescriptorSetIrradiance.h"
#include "FramebufferObjectIBL.h"
#include "GraphicsPipelineIrradiance.h"
#include "RenderPassIBL.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const uint8_t CUBE_FACES = 6;

const std::vector<glm::mat4> LOOK_AT_MATRICES =
{
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)),
    glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0))
};

const float PI = 3.14159265359;

//public
PrecomputedIBL::PrecomputedIBL() {}
PrecomputedIBL::~PrecomputedIBL() {}

void PrecomputedIBL::create(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
	Cube cube;
	//Panel panel;

	cube.init(logicalDevice, commandPool);
	//panel.init(logicalDevice, commandPool);

    irradianceCubeMap.create(logicalDevice,
        FBO_IBL_IRRADIANCE_EXTENT.y,
        FBO_IBL_IRRADIANCE_EXTENT.x,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        false);
	generateIrradianceCubeMap(logicalDevice, commandPool, cube);

    /*
	prefilteredCubeMap.create(renderer,
		FramebufferObjectPrefilter.IMAGE_EXTENT,
		FramebufferObjectPrefilter.IMAGE_EXTENT,
		VK_FORMAT_R16G16B16A16_SFLOAT,
		true);
	generatePrefilteredCubeMap(renderer, cube);

	integratedBRDF.create(renderer.getLogicalDevice(),
		FramebufferObjectIntegratedBRDF.IMAGE_EXTENT,
		FramebufferObjectIntegratedBRDF.IMAGE_EXTENT,
		VK_FORMAT_R16G16_SFLOAT,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SAMPLE_COUNT_1_BIT,
		false,
		VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE);
	generateIntegratedBRDF(renderer, panel);
    */

	//panel.deinit(logicalDevice);
	cube.deinit(logicalDevice);
}

void PrecomputedIBL::destroy(LogicalDevice& logicalDevice)
{
	irradianceCubeMap.destroy(logicalDevice);
	//integratedBRDF.destroy(logicalDevice);
	//prefilteredCubeMap.destroy(logicalDevice);
}

Texture* PrecomputedIBL::getIntegratedBRDF() { return &integratedBRDF; }
CubeMap* PrecomputedIBL::getIrradianceCubeMap() { return &irradianceCubeMap; }
CubeMap* PrecomputedIBL::getPrefilteredCubeMap() { return &prefilteredCubeMap; }

//private
void PrecomputedIBL::addDrawCommands(CommandBuffer& commandBuffer, VkDescriptorSet ds, GraphicsPipeline* gp, void* pc, uint32_t pcSize, VertexData* vertexData)
{
    VkBuffer vertexBuffer = vertexData->getVertexBuffer();
    VkDeviceSize offsets = 0;

    vkCmdPushConstants(commandBuffer.getCommandBuffer(), gp->getPipelineLayout(), gp->getPushConstantStages(), 0, pcSize, pc);

    vkCmdBindPipeline(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, gp->getPipeline());
    vkCmdBindDescriptorSets(commandBuffer.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, gp->getPipelineLayout(), 0, 1, &ds, 0, nullptr);

    vkCmdBindVertexBuffers(commandBuffer.getCommandBuffer(), 0, 1, &vertexBuffer, &offsets);
    vkCmdBindIndexBuffer(commandBuffer.getCommandBuffer(), vertexData->getIndexBuffer(), 0, vertexData->getIndexType());

    vkCmdDrawIndexed(commandBuffer.getCommandBuffer(), vertexData->getIndexCount(), 1, 0, 0, 0);
}

void PrecomputedIBL::generateIrradianceCubeMap(LogicalDevice& logicalDevice, CommandPool& commandPool, Cube& cube)
{
    GraphicsPipelineIrradiance gp;
    RenderPassIBL renderPass;
    FramebufferObjectIBL fbo(VK_FORMAT_R32G32B32A32_SFLOAT, FBO_IBL_IRRADIANCE_EXTENT.y, FBO_IBL_IRRADIANCE_EXTENT.x);
    DescriptorSetIrradiance ds;
    IrradiancePushConstant pc = {};
    CommandBuffer commandBuffer(commandPool);
    VkRenderPassBeginInfo renderPassBeginInfo = {};
    VkClearValue clearValues = {};
    VkRect2D renderArea = {};
    VkExtent2D extent = {};
    VkViewport viewport = {};
    VkRect2D scissor = {};

    renderPass.create(logicalDevice, VK_FORMAT_R32G32B32A32_SFLOAT);
    fbo.create(logicalDevice, &renderPass);
    gp.create(logicalDevice, renderPass.getRenderPass());
    ds.create(logicalDevice, &gp, cube.getCubeMap());

    clearValues.color = { 0.0f, 0.0f, 0.0f, 0.0f };

    extent.height = fbo.getHeight();
    extent.width = fbo.getWidth();

    renderArea.offset = { 0, 0 };
    renderArea.extent = extent;

    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass.getRenderPass();
    renderPassBeginInfo.renderArea = renderArea;
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValues;
    renderPassBeginInfo.framebuffer = fbo.getFramebuffer();

    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = extent.width;
    viewport.height = extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    scissor.offset = { 0, 0 };
    scissor.extent = extent;

    commandBuffer.beginOneTimeCommandBuffer(logicalDevice);

    vkCmdSetViewport(commandBuffer.getCommandBuffer(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer.getCommandBuffer(), 0, 1, &scissor);

    irradianceCubeMap.getImage().transitionLayout(logicalDevice,
        commandBuffer,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        false);

    for (uint32_t f = 0; f < CUBE_FACES; f++) {
        vkCmdBeginRenderPass(commandBuffer.getCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        pc.viewProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 96.0f);
        pc.viewProjectionMatrix[1][1] *= -1;
        pc.viewProjectionMatrix *= LOOK_AT_MATRICES[f];
        pc.deltaPhi = 2.0f * PI / 180.0f;
        pc.deltaTheta = 0.5f * PI / 64.0f;

        addDrawCommands(commandBuffer, ds.getDescriptorSet(), &gp, &pc, sizeof(IrradiancePushConstant), cube.getVertexData());

        vkCmdEndRenderPass(commandBuffer.getCommandBuffer());

        fbo.getColorAttachment().getImage().transitionLayout(logicalDevice,
            commandBuffer,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            false);

        fbo.getColorAttachment().getImage().copyToImage(irradianceCubeMap.getImage().getImage(), static_cast<uint32_t>(viewport.height), static_cast<uint32_t>(viewport.width), f, 0, commandBuffer);

        fbo.getColorAttachment().getImage().transitionLayout(logicalDevice,
            commandBuffer,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            false);
    }

    irradianceCubeMap.getImage().transitionLayout(logicalDevice,
        commandBuffer,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        false);

    commandBuffer.submitOneTimeCommandBuffer(logicalDevice);

    gp.destroy(logicalDevice);
    fbo.destroy(logicalDevice);
    renderPass.destroy(logicalDevice);
}