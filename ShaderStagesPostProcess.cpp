#include "ShaderStagesPostProcess.h"

//public
ShaderStagesPostProcess::ShaderStagesPostProcess() {}
ShaderStagesPostProcess::~ShaderStagesPostProcess() {}

//protected
const char* ShaderStagesPostProcess::getShaderPathFragment() { return SHADER_PATH_FRAGMENT; }
const char* ShaderStagesPostProcess::getShaderPathVertex() { return SHADER_PATH_VERTEX; }