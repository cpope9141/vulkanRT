#pragma once
#include <stb_image.h>

class RGBAResource
{
public:
	RGBAResource(int height, int width, stbi_uc* imageRGBA);
	~RGBAResource();

	int height;
	stbi_uc* imageRGBA;
	int width;
};