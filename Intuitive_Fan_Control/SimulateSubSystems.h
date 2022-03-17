#pragma once

#include <chrono>

#include "user_config.h"
#include "MockIPC.h"
#include "IPC_Interface.h"
#include "MockFanCtrlRegisters.h"
#include "FanRegInterface.h"
#include "MockSubSystem.h"


//using std::chrono::duration_cast;
//using std::chrono::milliseconds;
//using std::chrono::seconds;
//using std::chrono::system_clock;

	//timer typedefs
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;


class SimulateSubSystems
{
public:


	//default constructor
	SimulateSubSystems(MockIPC& IPC, MockFanCtrlRegisters& FAN_REG);
	
	//public attributes
	MockIPC& _IPC;
	MockFanCtrlRegisters& _FAN_REG;
	IPC_Interface _IPC_interface;
	FanRegInterface _fan_reg_interface;
	
	MockSubSystem* _sub_systems[USER_CFG.SYS_CNT_MAX]; //fixed size pointer array to mock sub-system objects
	uint8_t _cnt; //number of active sub-systems configured at startup
	TimePoint _t0; //init timer value at begin()
	float _sys_t; //live timer value in seconds since begin()

//public methods
	void add_subsystem();
	void add_subsystem(float ambient_temperature, float q_gen, float therm_coef, float therm_k_base, float therm_k_fan);
	void begin();
	void update();
	float get_fan_speed_from_reg(uint8_t idx);
	bool write_temp_to_ipc(float cur_temp, uint8_t idx);
	//void set_fan_speed(float fan_speed);

	static TimePoint get_cur_timepoint();
	static float get_elapsed_fsec(TimePoint last_t);
};

