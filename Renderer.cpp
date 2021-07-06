#include "Renderer.h"

#include "WindowManager.h"

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
	createSwapChainObjects();
	graphicsPipelinePostProcess.create(logicalDevice, swapChain.getRenderPass().getRenderPass());

	uboOrthoProjection.create(logicalDevice);
	uboOrthoProjection.update(logicalDevice, HEIGHT, WIDTH, 1.0f, 96.0f);
}

void Renderer::destroy()
{
    waitForDeviceIdle();

	uboOrthoProjection.destroy(logicalDevice);

	graphicsPipelinePostProcess.destroy(logicalDevice);
	destroySwapChainObjects();
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
        VkCommandBuffer commandBuffer = swapChain.getCommandBuffers()[nextImageIndex].getCommandBuffer();

        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        if (VK_SUCCESS == vkBeginCommandBuffer(commandBuffer, &beginInfo))
        {
            //ratracing commands here
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
void Renderer::createSwapChainObjects()
{
	swapChain.create(logicalDevice, commandPool);
}

void Renderer::destroySwapChainObjects()
{
	swapChain.destroy(logicalDevice, commandPool);
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