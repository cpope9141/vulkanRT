#pragma once
#include "Model.h"

#include "CubeMap.h"
#include "CubeVertexData.h"

class Cube : public Model
{
public:
	Cube();
	~Cube();

	void deinit(LogicalDevice logicalDevice);
	CubeMap* getCubeMap();
	void init(LogicalDevice& logicalDevice, CommandPool& commandPool);

private:
	CubeMap cubeMap = CubeMap(VK_FILTER_LINEAR);
	CubeVertexData cubeVertexData;
};