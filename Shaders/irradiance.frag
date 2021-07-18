#version 450

/*
 * constants
 */
const float PI = 3.14159265359;
const float TWO_PI = PI * 2.0;
const float HALF_PI = PI * 0.5;

/*
 * uniforms
 */
layout(binding = 0) uniform samplerCube environmentSampler;

layout(push_constant) uniform PushConsts {
    mat4 viewProjectionMatrix;
    float deltaPhi;
    float deltaTheta;
} pc;

/*
 * inputs
 */
layout(location = 0) in vec3 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec4 outputFragColor;

/*
 * functions
 */
void main() {
    vec3 irradiance = vec3(0.0);
    uint sampleCount = 0u;
    vec3 N = normalize(inputTexelCoords);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = cross(N, right);

    for(float phi = 0.0; phi < TWO_PI; phi += pc.deltaPhi) {
        for(float theta = 0.0; theta < HALF_PI; theta += pc.deltaTheta) {
            //spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            //tangent space to world
            vec3 sampleVector = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += texture(environmentSampler, sampleVector).rgb * cos(theta) * sin(theta);
            sampleCount++;
        }
    }
    
    outputFragColor = vec4(PI * irradiance / float(sampleCount), 1.0);
}