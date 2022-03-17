#pragma once

#include "user_config.h"
#include "IPC_Interface.h"
#include "FanRegInterface.h"

#define FCTRL_MIN_TEMP 25 //the lower temperature bound for the fan controller - duty_cycle = 20% for temps at/below this
#define FCTRL_MAX_TEMP 75 //the upper temperature bound for the fan controller - duty_cycle = 100% for temps at/above this
#define FCTRL_MIN_SPEED 20 //minimum duty_cycle of 20% for all fans when control enabled
#define FCTRL_MAX_SPEED 100 //maximum duty_cycle of 100% for all fans 

class FanController
{
public:
	//constructor requires references to pre-existing IPC and FAN_Register objects
	FanController(MockIPC& IPC, MockFanCtrlRegisters& FAN_REG);

	MockIPC& _IPC; //reference to existing IPC object
	MockFanCtrlRegisters& _FAN_REG; //reference to existing Fan Register object
	IPC_Interface _ipc_interface; //instantiates new instance of IPC interfacer
	FanRegInterface _fan_reg_interface; //instantiates new instance of the fan control register 

	const uint8_t _fan_cnt; //indicates the constant number of active systems - defined by user at startup and contained within exten USER_CFG struct
		//init after user config set

	bool _flag_enabled; //raise high/true to start fan controller, else disable controller

	void reset(); //called to stop or initialize the fan controller
	void setEnabled(bool enabled); //called to raise the _flag_enabled high or low for the update() method's state machine to act upon
	void update(); //called periodically to run real-time fan speed controller
private:
	float getIPCTemp(uint8_t idx);//convenience method for reading temperature values as floats from byte arrays stored in the IPC using the ipc_interface object
	bool setFanRegSpeed(float duty_cycle, uint8_t idx);//convenience method for writing fan pwm values associated with duty cycle to the Fan Register using the fan_reg_interface object
	float getMappedSpeed(float cur_temp, float min_temp, float max_temp, float min_speed, float max_speed); //given the current (max) temp, and associated temp/speed ranges - output interpolated fan duty_cycle
};

