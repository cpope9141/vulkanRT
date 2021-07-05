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
};