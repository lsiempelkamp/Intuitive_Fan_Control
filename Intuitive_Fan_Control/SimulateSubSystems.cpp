#include "SimulateSubSystems.h"

SimulateSubSystems::SimulateSubSystems(MockIPC& IPC, MockFanCtrlRegisters& FAN_REG)
:_IPC(IPC), _FAN_REG(FAN_REG), _IPC_interface(_IPC), _fan_reg_interface(_FAN_REG, USER_CFG.fan_cfg_lst)
{
	
	for (uint8_t i = 0; i < USER_CFG.SYS_CNT_MAX; i++)
	{
		_sub_systems[i] = NULL;
	}
	_cnt = 0;
	_t0 = get_cur_timepoint();
	_sys_t = 0.0f;
}

void SimulateSubSystems::add_subsystem()
{
	if (_cnt < USER_CFG.SYS_CNT_MAX)
	{
		_sub_systems[_cnt] = new MockSubSystem();
		_cnt++;
	}
}

void SimulateSubSystems::add_subsystem(float ambient_temperature, float q_gen, float therm_coef, float therm_k_base, float therm_k_fan)
{
	if (_cnt < USER_CFG.SYS_CNT_MAX)
	{
		_sub_systems[_cnt] = new MockSubSystem(ambient_temperature, q_gen, therm_coef, therm_k_base, therm_k_fan);
		_cnt++;
	}
}

void SimulateSubSystems::begin()
{
	_t0 = get_cur_timepoint();
	_sys_t = 0.0f;
	for (uint8_t i = 0; i < _cnt; i++)
	{
		_sub_systems[i]->begin(_sys_t);
	}
}

void SimulateSubSystems::update()
{
	for (uint8_t i = 0; i < _cnt; i++)
	{
		_sys_t = get_elapsed_fsec(_t0);
		_sub_systems[i]->set_fan_speed(get_fan_speed_from_reg(i));
		_sub_systems[i]->update(_sys_t);
		write_temp_to_ipc(_sub_systems[i]->_cur_temp, i);
	}
}


float SimulateSubSystems::get_fan_speed_from_reg(uint8_t idx)
{
	//return 0.0f;
	float fan_speed = 0.0f;
	if (_fan_reg_interface.getSpeed(&fan_speed, idx))
	{
		return fan_speed;
	}
	else
	{
		return 0.0f;
	}
}

bool SimulateSubSystems::write_temp_to_ipc(float cur_temp, uint8_t idx)
{
	return _IPC_interface.writeFloat(cur_temp, idx);
}

TimePoint SimulateSubSystems::get_cur_timepoint()
{
	return Time::now();
}

float SimulateSubSystems::get_elapsed_fsec(TimePoint last_t)
{
	fsec fs = Time::now() - last_t;
	return fs.count();
}
