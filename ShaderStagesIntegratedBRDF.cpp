#include "ShaderStagesIntegratedBRDF.h"

//public
ShaderStagesIntegratedBRDF::ShaderStagesIntegratedBRDF() {}
ShaderStagesIntegratedBRDF::~ShaderStagesIntegratedBRDF() {}

//protected
const char* ShaderStagesIntegratedBRDF::getShaderPathFragment() { return SHADER_PATH_FRAGMENT; }
const char* ShaderStagesIntegratedBRDF::getShaderPathVertex() { return SHADER_PATH_VERTEX; }