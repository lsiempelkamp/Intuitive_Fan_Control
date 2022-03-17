#pragma once

#include "user_config.h"
#include "MockFanCtrlRegisters.h"

class FanRegInterface
{
public:
	FanRegInterface(MockFanCtrlRegisters& FanCtrlReg, user_config::FanCfgLst& fan_cfg_lst);
	
	MockFanCtrlRegisters& _fan_registers;
	user_config::FanCfgLst& _fan_cfg_lst;

	bool getSpeed(float* duty_cycle, uint8_t idx);
	bool setSpeed(float duty_cycle, uint8_t idx);

};

