#pragma once
#include "ModelRT.h"

#include "StaticMeshVertexData.h"

class CerberusRT : public ModelRT
{
public:
	CerberusRT();
	~CerberusRT();

	void init(const LogicalDevice& logicalDevice, const CommandPool& commandPool);

private:
	StaticMeshVertexData staticMeshVertexData;
};