#pragma once

#include <cstring>

#include "user_config.h"
#include "MockIPC.h"

class IPC_Interface
{
public:
	IPC_Interface(MockIPC& IPC);

	MockIPC& _IPC;
	
	bool writeFloat(float f_val, uint8_t idx);
	bool readFloat(float* f_val, uint8_t idx);

};

