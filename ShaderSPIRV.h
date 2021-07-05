#pragma once
#include "shaderc/shaderc.h"
/*
const shaderc_shader_kind TYPE_CALLABLE_SHADER = shaderc_glsl_callable_shader;
const shaderc_shader_kind TYPE_CLOSEST_HIT_SHADER = shaderc_glsl_closesthit_shader;
const shaderc_shader_kind TYPE_FRAGMENT_SHADER = shaderc_glsl_fragment_shader;
const shaderc_shader_kind TYPE_MISS_SHADER = shaderc_glsl_miss_shader;
const shaderc_shader_kind TYPE_RAYGEN_SHADER = shaderc_glsl_raygen_shader;
const shaderc_shader_kind TYPE_VERTEX_SHADER = shaderc_glsl_vertex_shader;
*/

class ShaderSPIRV
{
public:
	ShaderSPIRV();
	~ShaderSPIRV();
/*
	const char* getBytecode();
	void compileShaderFile(const char* path, shaderc_shader_kind type);
	void free();

private:
	const char* bytecode;
	shaderc_compilation_result_t handle;
	*/
};