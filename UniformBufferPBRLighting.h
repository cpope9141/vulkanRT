#pragma once
#include "UniformBufferObject.h"

#include <glm/glm.hpp>

struct DirectionalLight {
    //glm::mat4 lightViewProjection;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 direction;
};

struct PositionalLight {
    alignas(16) glm::vec3 attenuation;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 position;
};

struct PBRLighting
{
    alignas(16) DirectionalLight directionalLight;
    alignas(16) PositionalLight positionalLights[2];
};

class UniformBufferPBRLighting : public UniformBufferObject
{
public:
	UniformBufferPBRLighting();
	~UniformBufferPBRLighting();

	void update(LogicalDevice& logicalDevice, DirectionalLight& directionalLight, PositionalLight positionalLights[2]);
};