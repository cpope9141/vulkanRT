#version 450

/*
 * constants
 */
const float PI = 3.1415926536;
const uint NUM_SAMPLES = 1024u;
const vec3 N = vec3(0.0, 0.0, 1.0);

/*
 * inputs
 */
layout(location = 0) in vec2 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec4 outputFragColor;

/*
 * function declarations
 */
float geometrySchlickSmithGGX(float NdotL, float NdotV, float roughness);
vec2 hammersley(uint i, uint N);
vec3 importanceSampleGGX(vec2 Xi, float roughness, vec3 normal);
vec2 integrateBRDF(float NdotV, float roughness);
float radicalInverseVanDerCorput(uint i);
float random(vec2 co);

/*
 * functions
 */
float geometrySchlickSmithGGX(float NdotL, float NdotV, float roughness) {
    float k = (roughness * roughness) / 2.0;
    float ggxL = NdotL / (NdotL * (1.0 - k) + k);
    float ggxV = NdotV / (NdotV * (1.0 - k) + k);
    
    return ggxL * ggxV;
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

vec2 integrateBRDF(float NdotV, float roughness) {
    vec3 V = vec3(sqrt(1.0 - NdotV * NdotV), 0.0, NdotV);
    vec2 iBRDF = vec2(0.0);
    
    for(uint i = 0u; i < NUM_SAMPLES; i++) {
        vec2 Xi = hammersley(i, NUM_SAMPLES);
        vec3 H = importanceSampleGGX(Xi, roughness, N);
        float VdotH = clamp(dot(V, H), 0.0, 1.0);
        vec3 L = normalize(2.0 * VdotH * H - V);
        float NdotL = clamp(dot(N, L), 0.0, 1.0);
        float NdotV = clamp(dot(N, V), 0.0, 1.0); 
        float HdotN = clamp(dot(H, N), 0.0, 1.0);

        if(NdotL > 0.0) {
            float G = geometrySchlickSmithGGX(NdotL, NdotV, roughness);
            float G_Vis = (G * VdotH) / (HdotN * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);
            iBRDF += vec2((1.0 - Fc) * G_Vis, Fc * G_Vis);
        }
    }
    
    return iBRDF / float(NUM_SAMPLES);
}

void main() {outputFragColor = vec4(integrateBRDF(inputTexelCoords.s, 1.0 - inputTexelCoords.t), 0.0, 1.0);}

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