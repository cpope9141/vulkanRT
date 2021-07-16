#version 450

/*
 * constants
 */
const float PI = 3.14159265359;
const int POSITIONAL_LIGHT_COUNT = 2;

/*
 * structs
 */
struct DirectionalLight {
    //mat4 lightViewProjection;
    vec3 color;
    vec3 direction;
};

struct PositionalLight {
    vec3 attenuation;
    vec3 color;
    vec3 position;
};

/*
 * uniforms
 */
layout(binding = 1) uniform UniformBufferPBRLighting {
    DirectionalLight directionalLight;
    PositionalLight positionalLights[POSITIONAL_LIGHT_COUNT];
} uboPBRLighting;

layout(push_constant) uniform PushConstant {
    vec3 cameraPosition;
} pc;

layout(binding = 3) uniform sampler2D albedoSampler;
layout(binding = 4) uniform sampler2D aoSampler;
//layout(binding = 5) uniform sampler2D integratedBRDFSampler;
//layout(binding = 6) uniform samplerCube irradianceSampler;
layout(binding = 5) uniform sampler2D metallicSampler;
layout(binding = 6) uniform sampler2D normalSampler;
//layout(binding = 9) uniform samplerCube prefilteredSampler;
layout(binding = 7) uniform sampler2D roughnessSampler;

/*
 * inputs
 */
layout(location = 0) in vec3 inputFragPosition;
layout(location = 1) in vec3 inputFragNormal;
layout(location = 2) in vec3 inputFragTangent;
layout(location = 3) in vec2 inputTexelCoords;

/*
 * outputs
 */
layout(location = 0) out vec4 outputFragColor;

/*
 * functions declarations
 */
//vec3 ambientReflectance(vec3 N, vec3 R, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo);
vec3 calculateNormal();
vec3 directReflectance(vec3 N, vec3 V, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo);
float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickSmithGGX(float NdotL, float NdotV, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness);

/*
 * functions
 */
 /*
vec3 ambientReflectance(vec3 N, vec3 R, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo) {
    float ao = texture(aoSampler, inputTexelCoords).r;
    vec3 F = fresnelSchlick(NdotV, F0, roughnessClamped);    
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallicClamped);     
    
    //diffuse component
    vec3 irradiance = texture(irradianceSampler, N).rgb;
    vec3 diffuse = irradiance * albedo;
    
    //specular component
    const float MAX_REFLECTION_LOD = 9.0;
    float lod = roughnessClamped * MAX_REFLECTION_LOD;
    vec3 prefilteredColor = textureLod(prefilteredSampler, R, lod).rgb;       
    vec2 brdf = texture(integratedBRDFSampler, vec2(NdotV, roughnessClamped)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    return (kD * diffuse + specular) * ao;
}
*/

vec3 calculateNormal() {
    vec3 tangentSpaceNormal = texture(normalSampler, inputTexelCoords).xyz * 2.0 - 1.0;

    vec3 N = normalize(inputFragNormal);
    vec3 T = normalize(inputFragTangent);
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentSpaceNormal);
} 

vec3 directReflectance(vec3 N, vec3 V, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo) {
    vec3 Lo = vec3(0.0);
    
    for(int i = 0; i < uboPBRLighting.positionalLights.length(); i++) {
        vec3 L = normalize(uboPBRLighting.positionalLights[i].position - inputFragPosition);
        vec3 H = normalize(V + L);
        float NdotL = clamp(dot(N, L), 0.0, 1.0); 
        float distance = length(uboPBRLighting.positionalLights[i].position - inputFragPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = uboPBRLighting.positionalLights[i].color * attenuation;        
        
        //Cook-Torrance BRDF
        float NDF = distributionGGX(N, H, roughnessClamped);        
        float G = geometrySchlickSmithGGX(NdotL, NdotV, roughnessClamped);      
        //vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0); TODO: compare
        vec3 F = fresnelSchlick(NdotV, F0);       
        
        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallicClamped);
        vec3 specular = (NDF * G * F) / (4.0 * NdotV * NdotL + 0.001); 
               
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }
    
    return Lo;
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = clamp(dot(N, H), 0.0, 1.0); 
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    
    return a2 / (PI * denom * denom);
}

float geometrySchlickSmithGGX(float NdotL, float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;    
    float ggxL = NdotL / (NdotL * (1.0 - k) + k);
    float ggxV = NdotV / (NdotV * (1.0 - k) + k);
    
    return ggxL * ggxV;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);}

vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main() {    
    //vec3 N = normalize(inputFragNormal);
    vec3 N = calculateNormal();
    vec3 V = normalize(pc.cameraPosition - inputFragPosition);
    vec3 R = reflect(-V, N);
    
    vec3 albedo = pow(texture(albedoSampler, inputTexelCoords).rgb, vec3(2.2));
    float metallic = texture(metallicSampler, inputTexelCoords).r;
    float roughness = texture(roughnessSampler, inputTexelCoords).r;
    
    float metallicClamped = clamp(metallic, 0.025, 1.0); 
    float roughnessClamped = clamp(roughness, 0.025, 1.0);
    
    vec3 F0 = mix(vec3(0.04), albedo, metallicClamped);//surface reflection at zero incidence
    float NdotV = clamp(dot(N, V), 0.0, 1.0);   
    vec3 Lo = directReflectance(N, V, NdotV, F0, metallicClamped, roughnessClamped, albedo);
    vec3 ambient = vec3(0.0);//ambientReflectance(N, R, NdotV, F0, metallicClamped, roughnessClamped, albedo);
   
    outputFragColor = vec4(ambient + Lo, 1.0); 
}   