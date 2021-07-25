#include "SceneStorageBuffer.h"

#include "CommandBuffer.h"
#include "MemoryUtilities.h"

//public
SceneStorageBuffer::SceneStorageBuffer() : TransferBufferObject(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) {}
SceneStorageBuffer::~SceneStorageBuffer() {}

void SceneStorageBuffer::create(LogicalDevice& logicalDevice, CommandPool& commandPool, std::vector<ModelRT>& models)
{
    std::vector<SceneDesc> ssb;
    ssb.resize(models.size());

    for (size_t i = 0; i < ssb.size(); i++)
    {
        ssb[i].model = models[i].prepareModelMatrix();
        ssb[i].normal = ssb[i].model;
        ssb[i].vertexAddress = models[i].getVertexDataPtr()->getVertexBufferDeviceAddress();
        ssb[i].indexAddress = models[i].getVertexDataPtr()->getIndexBufferDeviceAddress();
    }

    TransferBufferObject::create(logicalDevice, commandPool, ssb.data(), ssb.size() * sizeof(SceneDesc));
}