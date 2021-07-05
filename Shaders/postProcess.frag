#version 450

/*
 * constants
 */
const float EXPOSURE = 1.0f;
const float GAMMA = 2.2f;

/*
 * uniforms
 */
layout(binding = 1) uniform sampler2D outputImage; 

/*
 * inputs
 */
layout(location = 0) in vec2 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec4 outputFragColor;

/*
 * functions
 */
void main() {
    vec4 color = vec4(texture(outputImage, inputTexelCoords).rgb, 1.0);
       
    vec3 toneMapped = vec3(1.0) - exp(-color.rgb * EXPOSURE);

    outputFragColor = vec4(pow(toneMapped, vec3(1.0 / GAMMA)), 1.0); 
}