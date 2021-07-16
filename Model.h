#pragma once
#include "VertexData.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Model
{
public:
	Model();
	~Model();

	VertexData* getVertexData();
	glm::mat4 prepareModelMatrix();
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

protected:
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	VertexData* vertexData;
};