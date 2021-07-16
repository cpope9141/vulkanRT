#pragma once
#include "Model.h"

#include "CommandPool.h"
#include "LogicalDevice.h"
#include "Texture.h"

class ModelRT : public Model
{
public:
	ModelRT();
	~ModelRT();

	void deinit(LogicalDevice logicalDevice);
	Texture* getAlbedo();
	Texture* getAmbientOcclusion();
	Texture* getMetallic();
	Texture* getNormal();
	Texture* getRoughness();

protected:
	Texture albedo = Texture(VK_FILTER_LINEAR);
	Texture ao = Texture(VK_FILTER_LINEAR);
	Texture metallic = Texture(VK_FILTER_LINEAR);
	Texture normal = Texture(VK_FILTER_LINEAR);
	Texture roughness = Texture(VK_FILTER_LINEAR);

	void createTexture(LogicalDevice logicalDevice, CommandPool commandPool, const char* path, Texture& texture);
};