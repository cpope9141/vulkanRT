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

	void create(const LogicalDevice& logicalDevice, const CommandPool& commandPool);
	void destroy(const LogicalDevice& logicalDevice);
	Texture* getIntegratedBRDF();
	CubeMap* getIrradianceCubeMap();
	CubeMap* getPrefilteredCubeMap();

private:
	Texture integratedBRDF = Texture(VK_FILTER_LINEAR);
	CubeMap irradianceCubeMap = CubeMap(VK_FILTER_LINEAR);
	CubeMap prefilteredCubeMap = CubeMap(VK_FILTER_LINEAR);

	void addDrawCommands(CommandBuffer& commandBuffer, VkDescriptorSet ds, GraphicsPipeline* gp, void* pc, uint32_t pcSize, VertexData* vertexData);
	void generateIntegratedBRDF(const LogicalDevice& logicalDevice, const CommandPool& commandPool, Panel& panel);
	void generateIrradianceCubeMap(const LogicalDevice& logicalDevice, const CommandPool& commandPool, Cube& cube);
	void generatePrefilteredCubeMap(const LogicalDevice& logicalDevice, const CommandPool& commandPool, Cube& cube);
};