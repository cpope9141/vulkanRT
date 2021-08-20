#include "Cube.h"

#include "Types.h"

#include <iostream>

const char* SKYBOX_HDR_FILES[] = 
{ 
	"Texture/Environment/HDR/pisa_cube/px.hdr",
	"Texture/Environment/HDR/pisa_cube/nx.hdr",
	"Texture/Environment/HDR/pisa_cube/py.hdr",
	"Texture/Environment/HDR/pisa_cube/ny.hdr",
	"Texture/Environment/HDR/pisa_cube/pz.hdr",
	"Texture/Environment/HDR/pisa_cube/nz.hdr"
};

//public
Cube::Cube()
{
	vertexData = &cubeVertexData;
}

Cube::~Cube() {}

void Cube::deinit(const LogicalDevice& logicalDevice)
{
	cubeMap.destroy(logicalDevice);
	vertexData->destroy(logicalDevice);
}

CubeMap* Cube::getCubeMap() { return &cubeMap; }

void Cube::init(const LogicalDevice& logicalDevice, const CommandPool& commandPool)
{
	HDRResource resources[6];

	for (uint8_t i = 0; i < 6; i++)
	{
		int width, height, channels;
		float* imageHDR = stbi_loadf(SKYBOX_HDR_FILES[i], &width, &height, &channels, STBI_rgb_alpha);

		if (nullptr != imageHDR)
		{
			resources[i] = { height, imageHDR, width };
		}
		else
		{
			throw std::runtime_error("Failed to read image HDR");
		}
	}

	cubeMap.create(logicalDevice, commandPool, resources);

	for (uint8_t i = 0; i < 6; i++)
	{
		stbi_image_free(resources[i].imageHDR);
	}

	cubeVertexData.init(logicalDevice, commandPool);
}