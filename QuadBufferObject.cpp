#include "QuadBufferObject.h"

#include "MemoryUtilities.h"
#include "QuadVertexData.h"

#include <cstring>

//public
QuadBufferObject::QuadBufferObject() : TransferBufferObject(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {}
QuadBufferObject::~QuadBufferObject() {}