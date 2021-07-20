#pragma once
#include "CommandPool.h"
#include "Cube.h"
#include "CubeMap.h"
#include "GraphicsPipeline.h"
#include "LogicalDevice.h"
#include "Panel.h"

class PrecomputedIBL
{
public:
	PrecomputedIBL();
	~PrecomputedIBL();

	void create(LogicalDevice& logicalDevice, CommandPool& commandPool);
	void destroy(LogicalDevice& logicalDevice);
	Texture* getIntegratedBRDF();
	CubeMap* getIrradianceCubeMap();
	CubeMap* getPrefilteredCubeMap();

private:
	Texture integratedBRDF = Texture(VK_FILTER_LINEAR);
	CubeMap irradianceCubeMap = CubeMap(VK_FILTER_LINEAR);
	CubeMap prefilteredCubeMap = CubeMap(VK_FILTER_LINEAR);

	void addDrawCommands(CommandBuffer& commandBuffer, VkDescriptorSet ds, GraphicsPipeline* gp, void* pc, uint32_t pcSize, VertexData* vertexData);
	void generateIntegratedBRDF(LogicalDevice& logicalDevice, CommandPool& commandPool, Panel& panel);
	void generateIrradianceCubeMap(LogicalDevice& logicalDevice, CommandPool& commandPool, Cube& cube);
	void generatePrefilteredCubeMap(LogicalDevice& logicalDevice, CommandPool& commandPool, Cube& cube);
};