#version 450

/*
 * uniforms
 */
layout(push_constant) uniform PushConsts {
    mat4 viewProjectionMatrix;
    float roughness;
    uint sampleCount;
} pc;

/*
 * inputs
 */
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec2 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec3 outputTexelCoords;

/*
 * functions
 */
void main() {
    outputTexelCoords = inputPosition;
    
    gl_Position = pc.viewProjectionMatrix * vec4(inputPosition, 1.0);
}