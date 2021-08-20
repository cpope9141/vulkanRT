#pragma once
#include "TransferBufferObject.h"

#include "ModelRT.h"

#include <glm/glm.hpp>

struct SceneDesc {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 normal;
    uint64_t vertexAddress;
    uint64_t indexAddress;
};

class SceneStorageBuffer : public TransferBufferObject
{
public:
	SceneStorageBuffer();
	~SceneStorageBuffer();

    void create(const LogicalDevice& logicalDevice, const CommandPool& commandPool, std::vector<ModelRT>& models);
};