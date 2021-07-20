#pragma once
#include "ShaderStagesGraphics.h"

class ShaderStagesPrefiltered : public ShaderStagesGraphics
{
public:
	ShaderStagesPrefiltered();
	~ShaderStagesPrefiltered();

protected:
	virtual const char* getShaderPathFragment() override;
	virtual const char* getShaderPathVertex() override;

private:
	const char* SHADER_PATH_FRAGMENT = "Shaders/prefilter.frag.spv";
	const char* SHADER_PATH_VERTEX = "Shaders/prefilter.vert.spv";
};