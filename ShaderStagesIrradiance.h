#pragma once
#include "ShaderStagesGraphics.h"

class ShaderStagesIrradiance : public ShaderStagesGraphics
{
public:
	ShaderStagesIrradiance();
	~ShaderStagesIrradiance();

protected:
	virtual const char* getShaderPathFragment() override;
	virtual const char* getShaderPathVertex() override;

private:
	const char* SHADER_PATH_FRAGMENT = "Shaders/irradiance.frag.spv";
	const char* SHADER_PATH_VERTEX = "Shaders/irradiance.vert.spv";
};