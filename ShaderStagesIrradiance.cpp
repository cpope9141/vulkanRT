#include "ShaderStagesIrradiance.h"

//public
ShaderStagesIrradiance::ShaderStagesIrradiance() {}
ShaderStagesIrradiance::~ShaderStagesIrradiance() {}

//protected
const char* ShaderStagesIrradiance::getShaderPathFragment() { return SHADER_PATH_FRAGMENT; }
const char* ShaderStagesIrradiance::getShaderPathVertex() { return SHADER_PATH_VERTEX; }