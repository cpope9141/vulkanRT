#include "RGBAResource.h"

//public
RGBAResource::RGBAResource(int height, int width, stbi_uc* imageRGBA)
{
	this->height = height;
	this->imageRGBA = imageRGBA;
	this->width = width;
}

RGBAResource::~RGBAResource() {}