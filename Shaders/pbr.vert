#version 450

/*
 * constants
 */
const int POSITIONAL_LIGHT_COUNT = 2;

/*
 * uniforms
 */
layout(binding = 0) uniform UniformBufferProjection {
    mat4 matrix;
} uboProjection;

layout(binding = 2) uniform UniformBufferStaticModel {
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat4 normalMatrix;
} uboStaticModel;

/*
 * inputs
 */
layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec3 inputTangent;
layout(location = 3) in vec2 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec3 outputFragPosition;
layout(location = 1) out vec3 outputFragNormal;
layout(location = 2) out vec3 outputFragTangent;
layout(location = 3) out vec2 outputTexelCoords;

/*
 * functions
 */
void main() {
    vec4 worldPosition = uboStaticModel.modelMatrix * vec4(inputPosition, 1.0);
    
    outputTexelCoords = inputTexelCoords;
    outputFragNormal = mat3(uboStaticModel.modelMatrix) * inputNormal;
    outputFragTangent = mat3(uboStaticModel.modelMatrix) * inputTangent;
    outputFragPosition = worldPosition.xyz;
    
    gl_Position = uboProjection.matrix * uboStaticModel.viewMatrix * worldPosition;
}