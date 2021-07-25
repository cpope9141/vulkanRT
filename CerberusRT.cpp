#include "CerberusRT.h"

#include "ModelLoader.h"

#include <string>

const char* ALBEDO = "Model/Cerberus/Cerberus_A.png";
const char* AO = "Model/Cerberus/Cerberus_AO.png";
const char* METALLIC = "Model/Cerberus/Cerberus_M.png";
const char* NORMAL = "Model/Cerberus/Cerberus_N.png";
const char* ROUGHNESS = "Model/Cerberus/Cerberus_R.png";
const char* MODEL = "Model/Cerberus/cerberus.obj";

//public
CerberusRT::CerberusRT()
{
	vertexData = &staticMeshVertexData;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, -120.0f, 0.0f);
	scale = glm::vec3(5.0f);
}

CerberusRT::~CerberusRT() {}

void CerberusRT::init(LogicalDevice& logicalDevice, CommandPool& commandPool)
{
	ModelResource resource = loadModel(std::string(MODEL));
	createTexture(logicalDevice, commandPool, ALBEDO, albedo);
	createTexture(logicalDevice, commandPool, AO, ao);
	createTexture(logicalDevice, commandPool, METALLIC, metallic);
	createTexture(logicalDevice, commandPool, NORMAL, normal);
	createTexture(logicalDevice, commandPool, ROUGHNESS, roughness);

	staticMeshVertexData.init(logicalDevice, commandPool, resource);
	staticMeshVertexData.setDeviceAddresses(logicalDevice);
}