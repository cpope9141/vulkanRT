#pragma once
#include "DescriptorSet.h"

#include "GraphicsPipeline.h"
#include "ModelRT.h"
#include "Texture.h"
#include "UniformBufferPBRLighting.h"
#include "UniformBufferProjection.h"
#include "UniformBufferStaticModelPBR.h"

class DescriptorSetPBR : public DescriptorSet
{
public:
	DescriptorSetPBR();
	~DescriptorSetPBR();

	void create(LogicalDevice logicalDevice,
		GraphicsPipeline* gp,
		UniformBufferProjection* uboProjection,
		UniformBufferPBRLighting* uboLighting,
		UniformBufferStaticModelPBR* uboStaticModel,
		//PrecomputedIBL iblGeneratedMaps,
		ModelRT* modelRT);
	void destroy(LogicalDevice logicalDevice, GraphicsPipeline* gp);

protected:
	virtual void updateDescriptorSet(LogicalDevice logicalDevice) override;

private:
	Texture* albedo;
	Texture* ao;
	//Texture integratedBRDF;
	//CubeMap irradiance;
	Texture* metallic;
	Texture* normal;
	//CubeMap prefiltered;
	Texture* roughness;
	UniformBufferPBRLighting* uboLighting;
	UniformBufferProjection* uboProjection;
	UniformBufferStaticModelPBR* uboStaticModel;
};