#version 450

/*
 * uniforms
 */
layout(binding = 0) uniform UniformBufferObject {
    mat4 projection;
} ubo;

layout(push_constant) uniform PushConstant {
    mat4 modelView;
} pc;

/*
 * inputs
 */
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec2 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec2 outputTexelCoords;

/*
 * functions
 */
void main() {
    outputTexelCoords = inputTexelCoords;
    
    gl_Position = ubo.projection * pc.modelView * vec4(inputPosition, 1.0);
}