#include "IPC_Interface.h"

IPC_Interface::IPC_Interface(MockIPC& IPC)
:_IPC(IPC)
{}

bool IPC_Interface::writeFloat(float f_val, uint8_t idx)
{
	if (idx < _IPC._cnt)
	{
		memcpy(_IPC._elems[idx]._elem, &f_val, SIZE_FLOAT);
		return true;
	}
	return false;
}

bool IPC_Interface::readFloat(float* f_val, uint8_t idx)
{
	if (idx < _IPC._cnt && f_val != NULL)
	{
		memcpy(f_val, _IPC._elems[idx]._elem, SIZE_FLOAT);
		return true;
	}
	else
	{
		return false;
	}
}
