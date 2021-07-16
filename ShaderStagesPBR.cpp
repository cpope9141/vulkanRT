#include "ShaderStagesPBR.h"

//public
ShaderStagesPBR::ShaderStagesPBR() {}
ShaderStagesPBR::~ShaderStagesPBR() {}

//protected
const char* ShaderStagesPBR::getShaderPathFragment() { return SHADER_PATH_FRAGMENT; }
const char* ShaderStagesPBR::getShaderPathVertex() { return SHADER_PATH_VERTEX; }