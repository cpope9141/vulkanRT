#include "ShaderSPIRV.h"

#include <iostream>

//public
ShaderSPIRV::ShaderSPIRV()
{
    /*
    bytecode = nullptr;
    handle = nullptr;
    */
}

ShaderSPIRV::~ShaderSPIRV() {}

/*
const char* ShaderSPIRV::getBytecode() { return bytecode; }

void ShaderSPIRV::compileShaderFile(const char* path, shaderc_shader_kind type)
{
    if (nullptr == handle)
    {
        const char* absolutePath = nullptr;//getSystemClassLoader().getResource(shaderFile).toExternalForm();
        const char* source = nullptr;//new String(Files.readAllBytes(Paths.get(new URI(absolutePath))));
        shaderc_compiler_t compiler = shaderc_compiler_initialize();
        shaderc_compile_options_t options = shaderc_compile_options_initialize();

        shaderc_compile_options_set_target_env(options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

        if (nullptr == compiler)
        {
            throw std::runtime_error("Failed to create shader compiler");
        }
        else
        {
            shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, source, 0, type, absolutePath, "main", options);

            if (nullptr == result) { throw std::runtime_error("Failed to compile shader into SPIR-V"); }

            if (shaderc_compilation_status_success != shaderc_result_get_compilation_status(result))
            {
                throw std::runtime_error("ShaderC compilation does not have success status");
            }

            shaderc_compiler_release(compiler);

            handle = result;
            bytecode = shaderc_result_get_bytes(result);
        }
    }
    else
    {
        throw std::runtime_error("Only one shader can be compiled per instance");
    }
}

void ShaderSPIRV::free()
{
    shaderc_result_release(handle);
    bytecode = nullptr;
}
*/