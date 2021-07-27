#include "Renderer.h"

#include "Types.h"
#include "WindowManager.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

//public
Renderer::Renderer()
{
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 3.0f, -7.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

Renderer::~Renderer() {}

void Renderer::create()
{
	instance.create();
	physicalDevice.selectPhysicalDevice(instance.getInstance());
	logicalDevice.create(physicalDevice);
	commandPool.create(logicalDevice);

    loadAssets();
    createUniformBuffers();
	createSwapChainObjects();
}

void Renderer::destroy()
{
    waitForDeviceIdle();

	destroySwapChainObjects();
    destroyUniformBuffers();
    releaseAssets();
	commandPool.destroy(logicalDevice);
	logicalDevice.destroy();
	instance.destroy();
}

LogicalDevice Renderer::getLogicalDevice() { return logicalDevice; }

void Renderer::prepare()
{
    VkResult result = swapChain.acquireNextImage(logicalDevice);

    if (VK_SUCCESS == result)
    {
        uint32_t nextImageIndex = swapChain.getNextImageIndex();
        VkCommandBufferBeginInfo beginInfo = {};
        CommandBuffer commandBuffer = swapChain.getCommandBuffers()[nextImageIndex];

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (VK_SUCCESS == vkBeginCommandBuffer(commandBuffer.getCommandBuffer(), &beginInfo))
        {
            rayTracer.draw(logicalDevice, commandBuffer, nextImageIndex);

            /* alternatively draw using rasterization pipeline
            renderPassMultiSample.beginRenderPass(HEIGHT,
                WIDTH,
                fbosMultiSample[nextImageIndex].getFramebuffer(),
                commandBuffer);
            drawStaticModelPBR(cerberusRT);
            vkCmdEndRenderPass(commandBuffer);
            */
        }
        else
        {
            throw std::runtime_error("Failed to begin command buffer");
        }
    }
    else if (VK_ERROR_OUT_OF_DATE_KHR == result)
    {
        recreateSwapChain();
    }
    else 
    {
        throw std::runtime_error("Failed to acquire next image in swap chain");
    }
}

void Renderer::submit()
{
    uint32_t nextImageIndex = swapChain.getNextImageIndex();
    VkCommandBuffer commandBuffer = swapChain.getCommandBuffers()[nextImageIndex].getCommandBuffer();

    swapChain.getRenderPass().beginRenderPass(swapChain.getImageExtent().height,
        swapChain.getImageExtent().width,
        swapChain.getFramebuffers()[nextImageIndex].getFramebuffer(),
        commandBuffer);
    drawPostProcess();
    vkCmdEndRenderPass(commandBuffer);

    if (VK_SUCCESS == vkEndCommandBuffer(commandBuffer))
    {
        if (VK_SUCCESS == swapChain.submitDrawCommand(logicalDevice, commandBuffer))
        {
            VkResult result = swapChain.present(logicalDevice);

            if (VK_ERROR_OUT_OF_DATE_KHR == result || VK_SUBOPTIMAL_KHR == result || getFramebufferResized())
            {
                acknowledgeResize();
                recreateSwapChain();
            }
            else if (VK_SUCCESS != result)
            {
                throw std::runtime_error("Failed to present swap chain image");
            }
        }
        else
        {
            throw std::runtime_error("Failed to submit command buffer");
        }
    }
    else
    {
        throw std::runtime_error("Failed to end command buffer");
    }
}

void Renderer::waitForDeviceIdle() { vkDeviceWaitIdle(logicalDevice.getDevice()); }

//private
void Renderer::addDrawCommands(VkDescriptorSet ds, GraphicsPipeline* gp, void* pc, uint32_t pcSize, VertexData* vertexData)
{
    int nextImageIndex = swapChain.getNextImageIndex();
    VkCommandBuffer commandBuffer = swapChain.getCommandBuffers()[nextImageIndex].getCommandBuffer();
    VkBuffer vertexBuffer = vertexData->getVertexBuffer();
    VkDeviceSize offsets = 0;

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp->getPipeline());
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offsets);
    vkCmdBindIndexBuffer(commandBuffer, vertexData->getIndexBuffer(), 0, vertexData->getIndexType());
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gp->getPipelineLayout(), 0, 1, &ds, 0, nullptr);

    if (nullptr != pc)
    {
        vkCmdPushConstants(commandBuffer, gp->getPipelineLayout(), gp->getPushConstantStages(), 0, pcSize, pc);
    }

    vkCmdDrawIndexed(commandBuffer, vertexData->getIndexCount(), 1, 0, 0, 0);
}

void Renderer::createSwapChainObjects()
{
	swapChain.create(logicalDevice, commandPool);
    size_t swapChainImageCount = swapChain.getFramebuffers().size();
    fbosMultiSample.resize(swapChainImageCount);
    renderPassMultiSample.create(logicalDevice, VK_FORMAT_R32G32B32A32_SFLOAT);

    for (size_t i = 0; i < fbosMultiSample.size(); i++)
    {
        fbosMultiSample[i] = FramebufferObjectMultiSample(HEIGHT, WIDTH, VK_FORMAT_R32G32B32A32_SFLOAT);
        fbosMultiSample[i].create(logicalDevice, &renderPassMultiSample);
    }

    graphicsPipelinePBR.create(logicalDevice, renderPassMultiSample.getRenderPass());
    graphicsPipelinePostProcess.create(logicalDevice, swapChain.getRenderPass().getRenderPass());

    descriptorSetPBR.create(logicalDevice, &graphicsPipelinePBR, &uboPerspective, &uboLighting, &uboStaticModel, &precomputedIBL, &cerberusRT);
    descriptorSetPostProcess.resize(swapChainImageCount);

    rayTracer.create(logicalDevice, commandPool, precomputedIBL, models, viewMatrix, uboPerspective.getProjection(), glm::vec3(10, 5, 0), swapChainImageCount);
    std::vector<Texture> outputImages = rayTracer.getOutputImages();

    for (size_t i = 0; i < descriptorSetPostProcess.size(); i++)
    {
        Texture t = outputImages[i];
        descriptorSetPostProcess[i].create(logicalDevice, &graphicsPipelinePostProcess, &uboOrthographic, &t);
    }
    /* alternatively draw with rasterization pipeline
    for (size_t i = 0; i < descriptorSetPostProcess.size(); i++)
    {
        Texture t = fbosMultiSample[i].getColorAttachment();
        descriptorSetPostProcess[i].create(logicalDevice, &graphicsPipelinePostProcess, &uboOrthographic, &t);
    }
    */
}

void Renderer::createUniformBuffers()
{
    uboOrthographic.create(logicalDevice);
    uboOrthographic.update(logicalDevice, HEIGHT, WIDTH, 1.0f, 96.0f);
    uboPerspective.create(logicalDevice);
    uboPerspective.update(logicalDevice, HEIGHT, WIDTH, 1.0f, 96.0f);
    uboLighting.create(logicalDevice);
    PositionalLight positionalLight0 = {};
    positionalLight0.attenuation = glm::vec3(1, 0.14f, 0.07f);
    positionalLight0.color = glm::vec3(1);
    positionalLight0.position = glm::vec3(-1, 0, -1);
    PositionalLight positionalLight1 = {};
    positionalLight1.attenuation = glm::vec3(1, 0.14f, 0.07f);
    positionalLight1.color = glm::vec3(1);
    positionalLight1.position = glm::vec3(1, 0, -1);
    PositionalLight positionalLights[2] = { positionalLight0, positionalLight1 };
    DirectionalLight directionalLight = {};
    directionalLight.color = glm::vec3(1, 1, 1);
    directionalLight.direction = glm::vec3(-10, -5, 0);
    uboLighting.update(logicalDevice, directionalLight, positionalLights);
    uboStaticModel.create(logicalDevice);
    uboStaticModel.update(logicalDevice, viewMatrix, cerberusRT.prepareModelMatrix(), cerberusRT.prepareModelMatrix());
}

void Renderer::destroySwapChainObjects()
{
    descriptorSetPBR.destroy(logicalDevice, &graphicsPipelinePBR);

    for (DescriptorSetPostProcess dspp : descriptorSetPostProcess) { dspp.destroy(logicalDevice, &graphicsPipelinePostProcess); }
    descriptorSetPostProcess.clear();

    rayTracer.destroy(logicalDevice);

    graphicsPipelinePostProcess.destroy(logicalDevice);
    graphicsPipelinePBR.destroy(logicalDevice);
    for (FramebufferObjectMultiSample fboms : fbosMultiSample) { fboms.destroy(logicalDevice); }
    fbosMultiSample.clear();
    renderPassMultiSample.destroy(logicalDevice);
	swapChain.destroy(logicalDevice, commandPool);
}

void Renderer::destroyUniformBuffers()
{
    uboLighting.destroy(logicalDevice);
    uboStaticModel.destroy(logicalDevice);
    uboPerspective.destroy(logicalDevice);
    uboOrthographic.destroy(logicalDevice);
}

void Renderer::drawPostProcess()
{
    glm::mat4 pc;

    panel.setPosition(glm::vec3(0, 0, 0));
    panel.setScale(glm::vec3(WIDTH, HEIGHT, 1));
    pc = panel.prepareModelMatrix();

    addDrawCommands(descriptorSetPostProcess[swapChain.getNextImageIndex()].getDescriptorSet(), &graphicsPipelinePostProcess, &pc, sizeof(pc), panel.getVertexDataPtr());
}

void Renderer::drawStaticModelPBR(ModelRT staticModelPBR)
{
    glm::vec3 pc(0, 3, -7);

    addDrawCommands(descriptorSetPBR.getDescriptorSet(), &graphicsPipelinePBR, &pc, sizeof(pc), staticModelPBR.getVertexDataPtr());
}

void Renderer::loadAssets()
{
    panel.init(logicalDevice, commandPool);
    cerberusRT.init(logicalDevice, commandPool);
    precomputedIBL.create(logicalDevice, commandPool);

    models.resize(1);
    models[0] = cerberusRT;
}

void Renderer::recreateSwapChain()
{
    int height = 0;
    int width = 0;

    while (0 == width && 0 == height) {
        glfwGetFramebufferSize(getWindow(), &width, &height);
        glfwWaitEvents();
    }

    waitForDeviceIdle();
    destroySwapChainObjects();
    createSwapChainObjects();
}

void Renderer::releaseAssets()
{
    precomputedIBL.destroy(logicalDevice);
    cerberusRT.deinit(logicalDevice);
    panel.deinit(logicalDevice);
}