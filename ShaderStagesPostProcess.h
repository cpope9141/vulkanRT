#pragma once
#include "ShaderStagesGraphics.h"

class ShaderStagesPostProcess : public ShaderStagesGraphics
{
public:
	ShaderStagesPostProcess();
	~ShaderStagesPostProcess();

protected:
	virtual const char* getShaderPathFragment() override;
	virtual const char* getShaderPathVertex() override;

private:
	const char* SHADER_PATH_FRAGMENT = "Shaders/postProcess.frag.spv";
	const char* SHADER_PATH_VERTEX = "Shaders/postProcess.vert.spv";
};