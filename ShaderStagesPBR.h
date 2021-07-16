#pragma once
#include "ShaderStagesGraphics.h"

class ShaderStagesPBR : public ShaderStagesGraphics
{
public:
	ShaderStagesPBR();
	~ShaderStagesPBR();

protected:
	virtual const char* getShaderPathFragment() override;
	virtual const char* getShaderPathVertex() override;

private:
	const char* SHADER_PATH_FRAGMENT = "Shaders/pbr.frag.spv";
	const char* SHADER_PATH_VERTEX = "Shaders/pbr.vert.spv";
};