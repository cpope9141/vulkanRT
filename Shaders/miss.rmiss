#version 460
#extension GL_EXT_ray_tracing : require

/*
 * structs
 */
struct hitPayload {
    vec3 hitValue;
    int depth;
    vec3 attenuation;
    int done;
    vec3 rayOrigin;
    vec3 rayDir;
};

/*
 * uniforms
 */
layout(binding = 8) uniform samplerCube diffuseSampler;

/*
 * inputs
 */
layout(location = 0) rayPayloadInEXT hitPayload rayPayload;

/*
 * functions
 */
void main() {
    rayPayload.hitValue = texture(diffuseSampler, rayPayload.rayDir).rgb;
}