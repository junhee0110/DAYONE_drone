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
        PID * PID_instance[PID_NUM]; //PID instances
        motor * motor_instance[MOTOR_NUM];
        gy86 sensor;

        float current[PID_NUM];
        float current_rate[PID_NUM];

        float sampling_time = 0.0f; // dt
        float now_time, ex_time = 0.0f;
        

    public:
        control(PID * PID_array, motor * motor_array); //Initializer
        void init(); // Initialize Instance
        void cal_PID(); // Caluculate all PID values of each PID instance 
        void cal_Output(); // Calculate Output of each motor using PID value
        void get_sensor(); // Get setsor value from sensor module
        void set_current(); //Set current value of each PID instance
        void cal_sampling_time(); // Calculate Sampling Time by using milis()

};


#endif
