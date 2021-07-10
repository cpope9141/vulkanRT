#pragma once
#include "TransferBufferObject.h"

#include "LogicalDevice.h"
#include "CommandPool.h"

#include "Types.h"

#include <vector>

class QuadBufferObject : public TransferBufferObject
{
public:
	QuadBufferObject();
	~QuadBufferObject();
};