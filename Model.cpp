#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

//public
Model::Model()
{
	modelMatrix = glm::mat4(1.0f);
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	vertexData = nullptr;
}

Model::~Model() {}

VertexData* Model::getVertexData() { return vertexData; }

glm::mat4 Model::prepareModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

	return modelMatrix;
}

void Model::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Model::setScale(glm::vec3 scale)
{
	this->scale = scale;
}