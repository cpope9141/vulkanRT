#include "ModelRT.h"

#include "RGBAResource.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//public
ModelRT::ModelRT() {}
ModelRT::~ModelRT() {}

void ModelRT::deinit(LogicalDevice& logicalDevice)
{
	vertexData->destroy(logicalDevice);
	albedo.destroy(logicalDevice);
	ao.destroy(logicalDevice);
	metallic.destroy(logicalDevice);
	normal.destroy(logicalDevice);
	roughness.destroy(logicalDevice);
}

Texture* ModelRT::getAlbedoPtr() { return &albedo; }
Texture* ModelRT::getAmbientOcclusionPtr() { return &ao; }
Texture* ModelRT::getMetallicPtr() { return &metallic; }
Texture* ModelRT::getNormalPtr() { return &normal; }
Texture* ModelRT::getRoughnessPtr() { return &roughness; }

//protected
void ModelRT::createTexture(const LogicalDevice& logicalDevice, const CommandPool& commandPool, const char* path, Texture& texture)
{
	int width, height, channels;
	stbi_uc* imageRGBA = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);

	if (nullptr != imageRGBA)
	{
		RGBAResource rgbaResource = RGBAResource(height, width, imageRGBA);
		texture.create(logicalDevice, commandPool, rgbaResource);
		stbi_image_free(imageRGBA);
	}
	else
	{
		throw std::runtime_error("Failed to read image RGBA");
	}
}