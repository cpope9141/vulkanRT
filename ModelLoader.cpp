#include "ModelLoader.h"

#include <iostream>

const uint32_t ASSIMP_IMPORT_FIELD = (aiProcess_CalcTangentSpace |
    aiProcess_JoinIdenticalVertices |
    aiProcess_Triangulate |
    aiProcess_GenSmoothNormals |
    aiProcess_FlipUVs);

static void processIndices(const aiMesh* mesh, ModelResource& model);
static void processMesh(const aiMesh* mesh, ModelResource& model);
static void processNode(aiNode* node, const aiScene* scene, ModelResource& model);
static void processNodeMeshes(const aiScene* scene, aiNode* node, ModelResource& model);
static void processVertices(const aiMesh* mesh, ModelResource& model);

ModelResource loadModel(const std::string& file)
{
    Assimp::Importer importer;
    ModelResource model;

    const aiScene* scene = importer.ReadFile(file, ASSIMP_IMPORT_FIELD);

    if (nullptr != scene)
    {
        processNode(scene->mRootNode, scene, model);
    }
    else
    {
        std::string errorString(importer.GetErrorString());
        throw std::runtime_error("Assimp importer failed: " + errorString);
    }

    return model;
}

static void processIndices(const aiMesh* mesh, ModelResource& model)
{
    model.indices.resize(mesh->mNumFaces * 3);

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (uint32_t j = 0; j < face.mNumIndices; j++)
        {
            model.indices[i * 3 + j] = face.mIndices[j];
        }
    }
}

static void processMesh(const aiMesh* mesh, ModelResource& model)
{
    processIndices(mesh, model);
    processVertices(mesh, model);
}

void processNode(aiNode* node, const aiScene* scene, ModelResource& model)
{
    if (nullptr != node->mMeshes)
    {
        processNodeMeshes(scene, node, model);
    }

    if (nullptr != node->mChildren)
    {
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            processNode(*(node->mChildren + i), scene, model);
        }
    }
}

static void processNodeMeshes(const aiScene* scene, aiNode* node, ModelResource& model)
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, model);
    }
}

static void processVertices(const aiMesh* mesh, ModelResource& model)
{
    model.vertices.resize(mesh->mNumVertices);
    aiVector3D* uvw = mesh->mTextureCoords[0];

    for (size_t i = 0; i < model.vertices.size(); i++)
    {
        aiVector3D normal = mesh->mNormals[i];
        model.vertices[i].normal = glm::vec3(normal.x, normal.y, normal.z);

        aiVector3D position = mesh->mVertices[i];
        model.vertices[i].position = glm::vec3(position.x, position.y, position.z);

        aiVector3D tangent = mesh->mTangents[i];
        model.vertices[i].tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

        aiVector3D texelCoords = uvw[i];
        model.vertices[i].texelCoords = glm::vec2(texelCoords.x, texelCoords.y);
    }
}