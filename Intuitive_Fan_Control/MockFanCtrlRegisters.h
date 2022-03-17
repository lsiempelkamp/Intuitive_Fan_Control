#pragma once

#include "user_config.h"
struct MockFanCtrlRegisters
{
	static const uint8_t _cnt = USER_CFG.SYS_CNT_MAX;
	uint32_t _reg[_cnt];
	
	void reset()
	{
		for (uint8_t i = 0; i < _cnt; i++)
		{
			_reg[i] = 0;
		}
	}

	uint32_t& operator[](uint8_t idx)
	{
		return _reg[idx];
	}

	MockFanCtrlRegisters()
	{
		reset();
	}
};