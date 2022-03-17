#pragma once

#include "user_config.h"

class MockSubSystem
{
public:
//public attributes
    //config attributes
    const float _ambient_temperature;
    const float _therm_coef;
    const float _therm_k_base;
    const float _therm_k_fan;
    const float _q_gen;
    //process attributes
    float _cur_temp = 0.0f;
    float _fan_speed = 0.0f;

    //default constructor
    MockSubSystem();
    //overloaded constructor
    MockSubSystem(float ambient_temperature, float q_gen, float therm_coef, float therm_k_base, float therm_k_fan);

    //public methods
    void begin();
    void begin(float t);
    void update(float t);
    void set_fan_speed(float fan_speed);

private:
//private attributes
    float _last_t;
    float _last_temp;
    
//private methods
    float get_q_in();
    float get_q_out();
};
