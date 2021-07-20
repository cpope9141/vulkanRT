#pragma once
#include "ShaderStagesGraphics.h"

class ShaderStagesIntegratedBRDF : public ShaderStagesGraphics
{
public:
	ShaderStagesIntegratedBRDF();
	~ShaderStagesIntegratedBRDF();

protected:
	virtual const char* getShaderPathFragment() override;
	virtual const char* getShaderPathVertex() override;

private:
	const char* SHADER_PATH_FRAGMENT = "Shaders/integratedBRDF.frag.spv";
	const char* SHADER_PATH_VERTEX = "Shaders/integratedBRDF.vert.spv";
};