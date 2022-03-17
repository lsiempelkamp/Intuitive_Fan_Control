#pragma once

#include "user_config.h"

#define SIZE_FLOAT 4



struct MockIPC
{
	struct IPC_Float_Element
	{
		uint8_t _elem[SIZE_FLOAT];

		void reset()
		{
			for (uint8_t i = 0; i < SIZE_FLOAT; i++)
			{
				_elem[i] = 0;
			}
		}
	};

	static const uint8_t _cnt = USER_CFG.SYS_CNT_MAX;
	IPC_Float_Element _elems[_cnt];

	void reset()
	{
		for (uint8_t i = 0; i < _cnt; i++)
		{
			_elems[i].reset();
		}
	}

	MockIPC()
	{
		reset();
	}
};
