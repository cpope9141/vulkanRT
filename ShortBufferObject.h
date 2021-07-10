#pragma once
#include "TransferBufferObject.h"

#include "CommandPool.h"
#include "LogicalDevice.h"

class ShortBufferObject : public TransferBufferObject
{
public:
	ShortBufferObject();
	~ShortBufferObject();
};