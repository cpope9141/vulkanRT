#pragma once
#include "ModelRT.h"

#include "StaticMeshVertexData.h"

class CerberusRT : public ModelRT
{
public:
	CerberusRT();
	~CerberusRT();

	void init(LogicalDevice logicalDevice, CommandPool commandPool);

private:
	StaticMeshVertexData staticMeshVertexData;
};