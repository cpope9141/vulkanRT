#include "ShaderStagesPostProcess.h"

const char* SHADER_PATH_FRAGMENT = "Shaders/postProcess.frag.spv";
const char* SHADER_PATH_VERTEX = "Shaders/postProcess.vert.spv";

//public
ShaderStagesPostProcess::ShaderStagesPostProcess() {}
ShaderStagesPostProcess::~ShaderStagesPostProcess() {}

//protected
const char* ShaderStagesPostProcess::getShaderPathFragment() { return SHADER_PATH_FRAGMENT; }
const char* ShaderStagesPostProcess::getShaderPathVertex() {	return SHADER_PATH_VERTEX; }