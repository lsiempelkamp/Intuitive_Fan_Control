// Intuitive_Fan_Control.cpp : 
// Author: Lewis Siempelkamp 2022-03-16

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include "user_config.h"
#include "MockIPC.h"
#include "MockFanCtrlRegisters.h"
#include "SimulateSubSystems.h"
#include "FanController.h"


using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::min;
using std::max;

//definition for global/external user_cfg struct
user_config USER_CFG;
//global definition for single instance of Mock IPC
MockIPC IPC;
//global definition for single instance of Mock FAN_PWM_REGISTERS
MockFanCtrlRegisters FAN_REG;

//both the mock simulator and the fan controller receive references to the same instances of the Mock IPC and Fan Registers
    //the fan controller is constructed in main() after user setup
SimulateSubSystems simulator(IPC, FAN_REG);


//init helper functions for main() - defined below main()
void user_setup(); //console interfacing for user configurable parameters at startup
void gen_active_systems(); //called after user setup to spawn/dynamically allocate required systems for simulation
void report_sim_data(); //super basic data formatter for console reporting

int main()
{
    //userIO to define the USER_CONFIG attributes
    user_setup(); 

    //once configured - required instances of the active systems are spawned/dynamically allocated
    gen_active_systems(); 
    
    //construct a single instance of the fan controller
    FanController fan_ctrlr(IPC, FAN_REG);

    //init mock simulation and fan controller then start simulation/controller update loop
    simulator.begin();
    fan_ctrlr.setEnabled(false); //fan controller starts disabled

    //run the simulation and fan controller for the configured amount of time - initially with fan control disabled
    while (simulator._sys_t < USER_CFG.t_total_runtime)
    {
        //once the configured start timer for the fan controller has passed - enable the fan controller
        if (simulator._sys_t > USER_CFG.t_fan_control_start)
        {
            fan_ctrlr.setEnabled(true);
        }
        fan_ctrlr.update();
        simulator.update();
        report_sim_data();
        std::this_thread::sleep_for(std::chrono::milliseconds(USER_CFG.sim_time_step_ms));
    }
    cout << "SIM COMPLETE!" << endl << "Restart App to run again with different values" << endl;
    
    return 0;
}

void user_setup()
{
    int x;
    float f;

    cout << "Intuitive Fan Control Coding Challenge!" << endl;
    cout << "Author: Lewis Siempelkamp" << endl;
    cout << endl;
    
    cout << "Enter Ambient Temperature (" << (int)USER_CFG.AMBIENT_TEMP_MIN << "-" << (int)USER_CFG.AMBIENT_TEMP_MAX << ")" << endl;
    cin >> x;
    USER_CFG.ambient_temp = min(USER_CFG.AMBIENT_TEMP_MAX, max(USER_CFG.AMBIENT_TEMP_MIN, x));
    cout << "Ambient Temperature: " << USER_CFG.ambient_temp << endl;
    cout << endl;

    cout << "Enter number of active Sub Systems (" << (int)USER_CFG.SYS_CNT_MIN << "-" << (int)USER_CFG.SYS_CNT_MAX << ")" << endl;
    cin >> x;
    USER_CFG.sys_cnt = (uint8_t)min((int)USER_CFG.SYS_CNT_MAX, max((int)USER_CFG.SYS_CNT_MIN, x));
    cout << "Active Sub Systems: " << (int)USER_CFG.sys_cnt << endl;
    cout << endl;

    cout << "generating semi-random thermal and fan range properties for active SubSystems..." << endl;
    cout << setw(10) << "#" << setw(10) << "q_gen" << setw(10) << "fan_pwm_max" << endl;
    for (uint8_t i = 0; i < USER_CFG.sys_cnt; i++)
    {
        USER_CFG.therm_cfg_lst[i].set_q(rand() % (USER_CFG.Q_GEN_MAX - USER_CFG.Q_GEN_MIN) + USER_CFG.Q_GEN_MIN);
        USER_CFG.fan_cfg_lst[i] = rand() % (USER_CFG.FAN_PWM_MAX - USER_CFG.FAN_PWM_MIN) + USER_CFG.FAN_PWM_MIN;
        cout << setw(10) << (int)i << setw(10) << USER_CFG.therm_cfg_lst[i].q_gen << setw(10) << USER_CFG.fan_cfg_lst[i] << endl;
    }
    cout << endl;
    
    cout << "Enter time delay before Fan Controller Starts (eg 3.3 sec): " << endl;
    cin >> f;
    USER_CFG.t_fan_control_start = (float)f;
    cout << "Fan Controller Start Time is: " << USER_CFG.t_fan_control_start << endl;
    cout << endl;

    cout << "Enter Total Simulation Runtime (eg 10.2 sec): " << endl;
    cin >> f;
    USER_CFG.t_total_runtime = (float)f;
    cout << "Total Run Time is: " << USER_CFG.t_total_runtime << endl;
    cout << endl;

    cout << "Enter Simulation Timestep in milliseconds (eg 10-100): " << endl;
    cin >> x;
    USER_CFG.sim_time_step_ms = min(1000, max(1, x));
    cout << "Sim Time Step is: " << USER_CFG.sim_time_step_ms<< endl;
    cout << endl;

    cout << "CONFIG COMPLETE - STARTING SIMULATION" << endl;
    cout << endl << endl;

}

void gen_active_systems()
{
    for (uint8_t i = 0; i < USER_CFG.sys_cnt; i++)
    {
        simulator.add_subsystem(
            (float)USER_CFG.ambient_temp,
            (float)USER_CFG.therm_cfg_lst[i].q_gen,
            USER_CFG.therm_cfg_lst[i].therm_coeff,
            USER_CFG.therm_cfg_lst[i].therm_k_base,
            USER_CFG.therm_cfg_lst[i].therm_k_fan
        );
    }
}

void report_sim_data()
{
    static uint32_t cnt = 0;
    if (cnt == 0)
    {
        cout << setw(10) << "sim_time ";

        for (uint8_t i = 0; i < simulator._cnt; i++)
        {
            cout << "sys[" << (int)i << "].temp ";
        }

        for (uint8_t i = 0; i < simulator._cnt; i++)
        {
            cout << " IPC[" << (int)i << "].temp  ";
        }

        for (uint8_t i = 0; i < simulator._cnt; i++)
        {
            cout << "fan[" << (int)i << "].speed ";
        }

        for (uint8_t i = 0; i < simulator._cnt; i++)
        {
            cout << "fan[" << (int)i << "].pwm ";
        }

        cout << endl;
    }

    cout << setw(9) << simulator._sys_t << " ";
    
    for (uint8_t i = 0; i < simulator._cnt; i++)
    {
        cout << setw(11) << simulator._sub_systems[i]->_cur_temp << " ";
    }
    
    for (uint8_t i = 0; i < simulator._cnt; i++)
    {
        cout << " ";
        for (uint8_t j = 0; j < SIZE_FLOAT; j++)
        {
            //cout << std::hex << IPC._elems[i]._elem[j] << " ";
            printf("%.2X ", IPC._elems[i]._elem[j]);
        }
        cout << " ";
    }

    for (uint8_t i = 0; i < simulator._cnt; i++)
    {
        cout << setw(12) << simulator._sub_systems[i]->_fan_speed << " ";
    }

    for (uint8_t i = 0; i < simulator._cnt; i++)
    {
        cout << setw(10) << FAN_REG._reg[i] << " ";
    }
    
    cout << endl;
    cnt++;
}
