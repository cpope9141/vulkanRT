#include "ShaderStagesPrefiltered.h"

//public
ShaderStagesPrefiltered::ShaderStagesPrefiltered() {}
ShaderStagesPrefiltered::~ShaderStagesPrefiltered() {}

//protected
const char* ShaderStagesPrefiltered::getShaderPathFragment() { return SHADER_PATH_FRAGMENT; }
const char* ShaderStagesPrefiltered::getShaderPathVertex() { return SHADER_PATH_VERTEX; }