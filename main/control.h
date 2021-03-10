// File name: contorl.h
// Purpose: combine PID with motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-13
//------------- Edit log --------------------
// 20-02-09 Add control.h
// 20-02-13 Add Class Control

#ifndef CONTROL
#define CONTROL

#include "PID.h"
#include "config.h"
#include "motor.h"
#include "sensor.h"

class control
{

    private:
        PID ** PID_instance; //PID instances
        motor ** motor_instance;
        gy86 sensor;

        float current[PID_NUM]; //YAW PITCH ROLL X Y Z
        float current_rate[PID_NUM];

        float sampling_time = 0.0f; // dt
        float now_time, ex_time = 0.0f;
        

        int thrust;

    public:
        control(PID * PID_array[PID_NUM], motor * motor_array[MOTOR_NUM]); //Initializer
        void init(); // Initialize Instance
        void get_sensor(); // Get setsor value from sensor module
        void set_current(); //Set current value of each PID instance
        void cal_sampling_time(); // Calculate Sampling Time by using milis(), second unit
        void cal_PID(); // Caluculate all PID values of each PID instance 
        void cal_Output(); // Calculate Output of each motor using PID value

        void ctrl(); //Do 1 cycle

};


#endif
