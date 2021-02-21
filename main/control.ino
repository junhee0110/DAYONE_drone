// File name: contorl.ino
// Purpose: combine PID with motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-09
//------------- Edit log --------------------
// 20-02-09 Add control.ino
// 20-02-13 Add Class Control

#include "config.h"
#include "control.h"

control::control(PID * PID_array[PID_NUM], motor * motor_array(MOTOR_NUM))
{
    PID_instance = PID_array;
    motor_instance = motor_array;
}

void controll::init()
{
    //Init Motors
    for(int i = 0 ; i < MOTOR_NUM ; i++)
    {
        motor_instance[i]->init();
    }

    //Init Sensor
    sensor.init();
}

void control::cal_PID()
{
    for(int i = 0 ; i < PID_NUM ; i++)
    {
        PID_instance[i]->cal_PID();
    }
}

void control::cal_Output()
{

}

void control::get_sensor()
{
    sensor.get_dmp();
}

void control::set_current()
{
    for(int i = 0 ; i < PID_NUM ; i ++)
    {
        
    }
}

void control::cal_sampling_time()
{
    ex_time = now_time;
    //delayMicroseconds(50); //Add when you can't calculate sampling time
    now_time = micros();
    sampling_time = now_time - ex_time;
}