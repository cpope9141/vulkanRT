#pragma once
#include "Model.h"

#include "QuadVertexData.h"

class Panel : public Model
{
public:
	Panel();
	~Panel();

	void deinit(LogicalDevice& logicalDevice);
	void init(LogicalDevice& logicalDevice, CommandPool& commandPool);

private:
	QuadVertexData quadVertexData;
};