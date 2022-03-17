#include "FanController.h"

FanController::FanController(MockIPC& IPC, MockFanCtrlRegisters& FAN_REG)
:_IPC(IPC), _FAN_REG(FAN_REG), _ipc_interface(_IPC), _fan_reg_interface(_FAN_REG, USER_CFG.fan_cfg_lst), _fan_cnt(USER_CFG.sys_cnt)
{
	reset();

}

void FanController::reset()
{
	setEnabled(false);
	for (uint8_t i = 0; i < USER_CFG.SYS_CNT_MAX; i++)
	{
		setFanRegSpeed(0.0f, i); //clear all fan speed registers
	}
}

void FanController::setEnabled(bool enabled)
{
	_flag_enabled = enabled;
}

void FanController::update()
{
	if (!_flag_enabled)
	{
		reset();
		return;
	}
	else
	{
		//iterate through all active temperature readouts from the IPC
		//determine the max value to use as controller input
		float cur_max_temp = 0.0f;
		for (uint8_t i = 0; i < _fan_cnt; i++)
		{
			cur_max_temp = max(cur_max_temp, getIPCTemp(i));
		}
		
		//calculate the mapped/interpolated duty_cycle associated with the current max_temp
		float set_duty_cycle = getMappedSpeed(cur_max_temp, FCTRL_MIN_TEMP, FCTRL_MAX_TEMP, FCTRL_MIN_SPEED, FCTRL_MAX_SPEED);

		//set the fan registers for each active fan with the pwm value associated with the set_duty_cycle
		for (uint8_t i = 0; i < _fan_cnt; i++)
		{
			setFanRegSpeed(set_duty_cycle, i);
		}

		//HUZZAH!
	}
}

//private methods
float FanController::getIPCTemp(uint8_t idx)
{
	float latest_temp = 0.0f;
	if (_ipc_interface.readFloat(&latest_temp, idx))
	{
		return latest_temp;
	}
	else
	{
		return 0.0f;
	}
}

bool FanController::setFanRegSpeed(float duty_cycle, uint8_t idx)
{
	return _fan_reg_interface.setSpeed(duty_cycle, idx);
}

float FanController::getMappedSpeed(float cur_temp, float min_temp, float max_temp, float min_speed, float max_speed)
{
	//get the linear interpolated speed, then clamp it between the min and max speeds
	return min(max_speed, max(min_speed, (max_speed - min_speed) / (max_temp - min_temp) * (cur_temp - min_temp) + min_speed)) / 100.0f; 
}