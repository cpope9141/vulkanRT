#version 450

/*
 * constants
 */
const float PI = 3.14159265359;

/*
 * inputs
 */
layout(location = 0) in vec3 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec4 outputFragColor;

/*
 * uniforms
 */
layout(binding = 0) uniform samplerCube environmentSampler;

layout(push_constant) uniform PushConsts {
    mat4 viewProjectionMatrix;
    float roughness;
    uint sampleCount;
} pc;

/*
 * function declarations
 */
float distributionGGX(float NdotH, float roughness);
vec2 hammersley(uint i, uint N);
vec3 importanceSampleGGX(vec2 Xi, float roughness, vec3 normal);
vec3 prefilterEnvMap();
float radicalInverseVanDerCorput(uint i);
float random(vec2 co);

/*
 * functions
 */
float distributionGGX(float NdotH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    
    return a2 / (PI * denom * denom);
}

vec2 hammersley(uint i, uint N) {return vec2(float(i) / float(N), radicalInverseVanDerCorput(i));}

vec3 importanceSampleGGX(vec2 Xi, float roughness, vec3 normal) {
    //maps a 2D point to a hemisphere with spread based on roughness
    float alpha = roughness * roughness;
    float phi = 2.0 * PI * Xi.x + random(normal.xz) * 0.1;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (alpha * alpha - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    vec3 H = vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);

    //tangent space
    vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = normalize(cross(normal, tangent));

    //convert to world space
    return normalize(tangent * H.x + bitangent * H.y + normal * H.z);
}

void main() {outputFragColor = vec4(prefilterEnvMap(), 1.0);}

vec3 prefilterEnvMap() {
    vec3 N = normalize(inputTexelCoords);
    vec3 R = N;
    vec3 V = R;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;
    float envMapDimension = float(textureSize(environmentSampler, 0).s);
    
    for(uint i = 0u; i < pc.sampleCount; i++) {
        //generate a sample vector biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = hammersley(i, pc.sampleCount);
        vec3 H = importanceSampleGGX(Xi, pc.roughness, N);
        float VdotH = clamp(dot(V, H), 0.0, 1.0);
        vec3 L = normalize(2.0 * VdotH * H - V);
        float NdotL = clamp(dot(N, L), 0.0, 1.0);
        
        if(NdotL > 0.0) {
            float NdotH = clamp(dot(N, H), 0.0, 1.0);
            float pdf = distributionGGX(NdotH, pc.roughness) * NdotH / (4.0 * VdotH) + 0.0001;
            //solid angle of current sample
            float omegaS = 1.0 / (float(pc.sampleCount) * pdf + 0.0001);
            //solid angle of 1 pixel across all cube faces
            float omegaP = 4.0 * PI / (6.0 * envMapDimension * envMapDimension);
            //biased (+1.0) mip level for better result
            float mipLevel = pc.roughness == 0.0 ? 0.0 : max(0.5 * log2(omegaS / omegaP) + 1.0, 0.0f); 
            
            prefilteredColor += textureLod(environmentSampler, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }
    
    return (prefilteredColor / totalWeight);
}

float radicalInverseVanDerCorput(uint i) {
    uint bits = (i << 16u) | (i >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; //0x100000000
}

float random(vec2 co) {
    float a = 12.9898;
    float b = 78.233;
    float c = 43758.5453;
    float dt = dot(co.xy, vec2(a, b));
    float sn = mod(dt, 3.14);
    return fract(sin(sn) * c);
}