#pragma once
#include "DescriptorSet.h"

#include "AccelerationStructure.h"
#include "CubeMap.h"
#include "LogicalDevice.h"
#include "PrecomputedIBL.h"
#include "RayTracingPipelinePBR.h"
#include "SceneStorageBuffer.h"
#include "Texture.h"
#include "UniformBufferRT.h"

class DescriptorSetRTPBR : public DescriptorSet
{
public:
	DescriptorSetRTPBR();
	~DescriptorSetRTPBR();

	void create(LogicalDevice& logicalDevice,
		RayTracingPipelinePBR* rtp,
		Texture* storageImage,
		AccelerationStructure* tlas,
		UniformBufferRT* uboRT,
		SceneStorageBuffer* sceneStorageBuffer,
		std::vector<Texture*>& textures,
		PrecomputedIBL* precomputedIBL,
		CubeMap* environment);

protected:
	virtual void updateDescriptorSet(const LogicalDevice& logicalDevice) override;

private:
	SceneStorageBuffer* sceneStorageBuffer;
	Texture* storageImage;
	AccelerationStructure* tlas;
	UniformBufferRT* uboRT;

	std::vector<Texture*> textures;

	Texture* integratedBRDF;
	CubeMap* irradiance;
	CubeMap* prefiltered;

	CubeMap* environment;
};