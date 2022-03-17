#pragma once
#include <cstdint>
#include <algorithm>

using std::min;
using std::max;

struct user_config
{
	static const uint8_t SYS_CNT_MAX = 8;
	static const uint8_t SYS_CNT_MIN = 1;
	uint8_t sys_cnt = SYS_CNT_MIN; //user configurable

	static const int AMBIENT_TEMP_MAX = 30;
	static const int AMBIENT_TEMP_MIN = 10;
	int ambient_temp = 20; //user configurable

	//arbitrary thermal value ranges and coefficients 
	static const int Q_GEN_MIN = 50;
	static const int Q_GEN_MAX = 100;
	struct ThermCfg
	{
		float therm_coeff = 1.0f;
		float therm_k_base = 1.0f;
		float therm_k_fan = 1.5f;
		int q_gen = Q_GEN_MIN;

		void set_q(int new_q)
		{
			q_gen = min(Q_GEN_MAX, max(Q_GEN_MIN, new_q));
		}
	};
	struct ThermCfgLst
	{
		static const uint8_t _cnt = SYS_CNT_MAX;
		ThermCfg _elem[_cnt];
		ThermCfg& operator[](uint8_t idx)
		{
			return _elem[idx];
		}
	} therm_cfg_lst; //contents set for active sub systems at startup

	

	//fan pwm ranges
	static const uint32_t FAN_PWM_MIN = 1;
	static const uint32_t FAN_PWM_MAX = UINT32_MAX;
	struct FanCfgLst
	{
		static const uint8_t _cnt = SYS_CNT_MAX;
		uint32_t _elem[_cnt];
		uint32_t& operator[](uint8_t idx)
		{
			return _elem[idx];
		}
	} fan_cfg_lst = {}; //arbitarily set for active sub systems at startup

	//configurable run-timers for simulation
	float t_fan_control_start = 10.0f;
	float t_total_runtime = 20.0f;
	uint32_t sim_time_step_ms = 50;
};

extern user_config USER_CFG;