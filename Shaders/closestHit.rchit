#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : require

/*
 * constants
 */
const float PI = 3.14159265359;

const uint SAMPLER_2D_OFFSET_ALBEDO = 0;
const uint SAMPLER_2D_OFFSET_AO = 1;
const uint SAMPLER_2D_OFFSET_METALLIC = 2;
const uint SAMPLER_2D_OFFSET_NORMAL = 3;
const uint SAMPLER_2D_OFFSET_ROUGHNESS = 4;
const uint SAMPLER_2D_INSTANCE_STRIDE = 5;

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

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texelCoords;
};

struct SceneDesc {
    mat4 model;
    mat4 normal;
    uint64_t vertexAddress;
    uint64_t indexAddress;
};

/*
 * uniforms
 */
layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;

layout(binding = 2, set = 0) uniform UniformBufferRTShadow {
    mat4 viewInverse;
    mat4 projInverse;
    vec4 lightPos;
    int vertexSize;
} ubo;

/*
 * inputs
 */
layout(location = 0) rayPayloadInEXT hitPayload rayPayload;
layout(buffer_reference, scalar) buffer Vertices {float v[];};
layout(buffer_reference, scalar) buffer Indices {uint i[];};
layout(binding = 3) buffer SceneDesc_ {SceneDesc i[];} sceneDesc;
layout(binding = 4) uniform sampler2D textureSamplers[];
layout(binding = 5) uniform sampler2D integratedBRDFSampler;
layout(binding = 6) uniform samplerCube irradianceSampler;
layout(binding = 7) uniform samplerCube prefilteredSampler;

hitAttributeEXT vec3 attribs;

/*
 * outputs
 */
layout(location = 1) rayPayloadEXT bool shadowed;

/*
 * function declarations
 */
vec3 ambientReflectance(vec3 N, vec3 R, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo, vec2 texelCoords);
vec3 calculateNormal(vec2 texelCoords, vec3 normal, vec3 tangent);
vec3 directReflectance(vec3 N, vec3 V, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo, vec3 position);
float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickSmithGGX(float NdotL, float NdotV, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness);
Vertex unpack(Vertices vertices, uint index);

/*
 * functions
 */
vec3 ambientReflectance(vec3 N, vec3 R, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo, vec2 texelCoords) {
    float ao = texture(textureSamplers[gl_InstanceCustomIndexEXT * SAMPLER_2D_INSTANCE_STRIDE + SAMPLER_2D_OFFSET_AO], texelCoords).r;
    vec3 F = fresnelSchlick(NdotV, F0, roughnessClamped);    
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallicClamped);     
    
    //diffuse component
    vec3 irradiance = texture(irradianceSampler, N).rgb;
    vec3 diffuse = irradiance * albedo;

    return (kD * diffuse) * ao;
}

vec3 calculateNormal(vec2 texelCoords, vec3 normal, vec3 tangent) {
    uint tsIndex = (gl_InstanceCustomIndexEXT * SAMPLER_2D_INSTANCE_STRIDE) + SAMPLER_2D_OFFSET_NORMAL;
    vec3 tangentSpaceNormal = texture(textureSamplers[tsIndex], texelCoords).xyz * 2.0 - 1.0;

    vec3 N = normalize(normal);
    vec3 T = normalize(tangent);
    vec3 B = normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * tangentSpaceNormal);
}

vec3 directReflectance(vec3 N, vec3 V, float NdotV, vec3 F0, float metallicClamped, float roughnessClamped, vec3 albedo, vec3 position) {
    vec3 Lo = vec3(0.0);
    
    //for(int i = 0; i < 1; i++) {
        vec3 L = normalize(vec3(ubo.lightPos.xyz));// - vec4(position, 0.0)));
        vec3 H = normalize(V + L);
        float NdotL = clamp(dot(N, L), 0.0, 1.0);
        vec3 radiance = vec3(2.0);        
        
        //Cook-Torrance BRDF
        float NDF = distributionGGX(N, H, roughnessClamped);        
        float G = geometrySchlickSmithGGX(NdotL, NdotV, roughnessClamped);      
        //vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0); TODO: compare
        vec3 F = fresnelSchlick(NdotV, F0);       
        
        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallicClamped);
        vec3 specular = (NDF * G * F) / (4.0 * NdotV * NdotL + 0.001); 
               
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    //}
    
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

Vertex unpack(Vertices vertices, uint index) {
    Vertex v;
    uint offset = index * ubo.vertexSize / 4;
    
    v.position = vec3(vertices.v[offset], vertices.v[offset + 1], vertices.v[offset + 2]);
    v.normal = vec3(vertices.v[offset + 3], vertices.v[offset + 4], vertices.v[offset + 5]);
    v.tangent = vec3(vertices.v[offset + 6], vertices.v[offset + 7], vertices.v[offset + 8]);
    v.texelCoords = vec2(vertices.v[offset + 9], vertices.v[offset + 10]);
    
    return v;
}

/*
 * main
 */
void main() {
    SceneDesc objResource = sceneDesc.i[gl_InstanceCustomIndexEXT];
    Indices indices = Indices(objResource.indexAddress);
    Vertices vertices = Vertices(objResource.vertexAddress);
    
    ivec3 index = ivec3(indices.i[3 * gl_PrimitiveID], indices.i[3 * gl_PrimitiveID + 1], indices.i[3 * gl_PrimitiveID + 2]);

    Vertex v0 = unpack(vertices, index.x);
    Vertex v1 = unpack(vertices, index.y);
    Vertex v2 = unpack(vertices, index.z);

    const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);
        
    vec3 interpPosition = v0.position * barycentricCoords.x + v1.position * barycentricCoords.y + v2.position * barycentricCoords.z;
    interpPosition = vec3(objResource.model * vec4(interpPosition, 1.0));
    
    vec3 interpNormal = v0.normal * barycentricCoords.x + v1.normal * barycentricCoords.y + v2.normal * barycentricCoords.z;
    interpNormal = normalize(vec3(objResource.model * vec4(interpNormal, 0.0)));
    
    vec3 interpTangent = v0.tangent * barycentricCoords.x + v1.tangent * barycentricCoords.y + v2.tangent * barycentricCoords.z;
    interpTangent = normalize(vec3(objResource.model * vec4(interpTangent, 0.0)));
    
    vec2 interpTexelCoords = v0.texelCoords * barycentricCoords.x + v1.texelCoords * barycentricCoords.y + v2.texelCoords * barycentricCoords.z;
    
    vec3 viewPosition = rayPayload.rayOrigin;   

    vec3 N = calculateNormal(interpTexelCoords, interpNormal, interpTangent);
    vec3 V = normalize(viewPosition - interpPosition);
    vec3 R = reflect(-V, N);
    
    vec3 albedo = pow(texture(textureSamplers[(gl_InstanceCustomIndexEXT * SAMPLER_2D_INSTANCE_STRIDE) + SAMPLER_2D_OFFSET_ALBEDO], interpTexelCoords).rgb, vec3(2.2));
    float metallic = texture(textureSamplers[(gl_InstanceCustomIndexEXT * SAMPLER_2D_INSTANCE_STRIDE) + SAMPLER_2D_OFFSET_METALLIC], interpTexelCoords).r;
    float roughness = texture(textureSamplers[(gl_InstanceCustomIndexEXT * SAMPLER_2D_INSTANCE_STRIDE) + SAMPLER_2D_OFFSET_ROUGHNESS], interpTexelCoords).r;
    
    float metallicClamped = clamp(metallic, 0.025, 1.0); 
    float roughnessClamped = clamp(roughness, 0.025, 1.0);
    
    vec3 F0 = mix(vec3(0.04), albedo, metallicClamped);//surface reflection at zero incidence
    float NdotV = clamp(dot(N, V), 0.0, 1.0);
    vec3 Lo = directReflectance(N, V, NdotV, F0, metallicClamped, roughnessClamped, albedo, interpPosition);
    vec3 ambient = ambientReflectance(N, R, NdotV, F0, metallicClamped, roughnessClamped, albedo, interpTexelCoords);

    vec3 lightVector = normalize(ubo.lightPos.xyz);
    rayPayload.hitValue = ambient + Lo;
 
    // Shadow casting
    float tmin = 0.001;
    float tmax = 10000.0;
    vec3 origin = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
    shadowed = true;
      
    // Trace shadow ray and offset indices to match shadow hit/miss shader group indices
    traceRayEXT(topLevelAS,
        gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT,
        0xFF,
        1,
        0,
        1,
        origin,
        tmin,
        lightVector,
        tmax,
        1);
    
    if(shadowed) {rayPayload.hitValue *= 0.33;}
    
    if(0.025 <= metallicClamped * (1 - roughnessClamped)) {
        float ao = texture(textureSamplers[gl_InstanceCustomIndexEXT * SAMPLER_2D_INSTANCE_STRIDE + SAMPLER_2D_OFFSET_AO], interpTexelCoords).r;
        vec3 F = fresnelSchlick(NdotV, F0, roughnessClamped);
            
        rayPayload.attenuation *= F * ao;
        rayPayload.done = 0;
        rayPayload.rayOrigin = interpPosition;
        rayPayload.rayDir = reflect(gl_WorldRayDirectionEXT, N);
    }
}