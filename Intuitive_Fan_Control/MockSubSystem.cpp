#include "MockSubSystem.h"

MockSubSystem::MockSubSystem()
:_ambient_temperature(20.0), _q_gen(100.0), _therm_coef(1.0), _therm_k_base(1.0), _therm_k_fan(1.0)
{
	_cur_temp = 0.0f;
	_fan_speed = 0.0f;
	_last_t = 0.0f;
	_last_temp = 0.0f;
}

MockSubSystem::MockSubSystem(float ambient_temperature, float q_gen, float therm_coef, float therm_k_base, float therm_k_fan)
:_ambient_temperature(ambient_temperature), _q_gen(q_gen), _therm_coef(therm_coef), _therm_k_base(therm_k_base), _therm_k_fan(therm_k_fan)
{
	_cur_temp = 0.0f;
	_fan_speed = 0.0f;
	_last_t = 0.0f;
	_last_temp = 0.0f;
}

void MockSubSystem::begin()
{
	begin(0.0);
}

void MockSubSystem::begin(float t)
{
	_last_t = t;
	_last_temp = _ambient_temperature;
	_cur_temp = _last_temp;
	_fan_speed = 0.0;
	update(t);
}

void MockSubSystem::update(float t)
{
	float dt = t - _last_t; //timestep
	if (dt > 0.001f)
	{
		_last_t = t;
		float delta_temp = (get_q_in() - get_q_out()) * _therm_coef * dt;
		_cur_temp = _last_temp + delta_temp;
		_last_temp = _cur_temp;
	}
}

void MockSubSystem::set_fan_speed(float fan_speed)
{
	_fan_speed = fan_speed;
}

//private methods
float MockSubSystem::get_q_in()
{
	return (float)_q_gen;
}

float MockSubSystem::get_q_out()
{
	return (_cur_temp - _ambient_temperature) * (_therm_k_base + _therm_k_fan * _fan_speed);
}