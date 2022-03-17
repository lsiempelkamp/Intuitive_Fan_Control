#include "FanRegInterface.h"

FanRegInterface::FanRegInterface(MockFanCtrlRegisters& fan_registers, user_config::FanCfgLst& fan_cfg_lst)
:_fan_registers(fan_registers), _fan_cfg_lst(fan_cfg_lst)
{
}

bool FanRegInterface::getSpeed(float* duty_cycle, uint8_t idx)
{
	if (idx < _fan_registers._cnt && duty_cycle != NULL)
	{
		float new_duty_cycle = (float)_fan_registers[idx] / (float)_fan_cfg_lst[idx];
		*duty_cycle = new_duty_cycle;
		return true;
	}
	else
	{
		return false;
	}
}

bool FanRegInterface::setSpeed(float duty_cycle, uint8_t idx)
{
	if (idx < _fan_registers._cnt)
	{
		uint32_t new_speed = (uint32_t)(duty_cycle * (float)_fan_cfg_lst[idx]);
		_fan_registers[idx] = new_speed;
		return true;
	}
	else
	{
		return false;
	}
}