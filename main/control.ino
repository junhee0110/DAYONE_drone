// File name: contorl.ino
// Purpose: combine PID with motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-09
//------------- Edit log --------------------
// 20-02-09 Add control.ino
// 20-02-13 Add Class Control

#include "config.h"
#include "control.h"

control::control(PID * PID_array[3], motor * motor_array[3])
{
    PID_instance = PID_array;
    motor_instance = motor_array;
}

void control::init()
{
    //Init Motors
    for(int i = 0 ; i < MOTOR_NUM ; i++)
    {
        motor_instance[i]->init();
    }

    //Init Sensor
    sensor.init();
}

void control::get_sensor()
{
    sensor.get_dmp(); //roll,pitch,yaw 받아왔음 단, yaw는 heading으로
    current[0] = sensor.get_head();
    current[1] = sensor.get_pitch();
    current[2] = sensor.get_roll();
    current_rate[0] = sensor.getRotationZ(); //YAW
    current_rate[1] = sensor.getRotationY(); //PITCH
    current_rate[2] = sensor.getRotationX(); //ROLL
}



void control::cal_sampling_time()
{
    ex_time = now_time;
    //delayMicroseconds(50); //Add when you can't calculate sampling time
    now_time = micros();
    sampling_time = (now_time - ex_time) / 1000;
}

void control::set_current()
{
    for (int i = 0 ; i < PID_NUM ; i++)
    {
        switch(PID_instance[i] -> get_num())
        {
            case 0: //YAW
                PID_instance[i] -> set_first_target(current[0]);
                PID_instance[i] -> set_rate_target(current_rate[0]);
                break;
            
            case 1: //PITCH
                PID_instance[i] -> set_first_target(current[1]);
                PID_instance[i] -> set_rate_target(current_rate[1]);
                break;

            case 2: //ROLL
                PID_instance[i] -> set_first_target(current[2]);
                PID_instance[i] -> set_rate_target(current_rate[2]);
                break;

            case 3: //X
                
                break;

            case 4: //Y
                
                break;

            case 5: //Z
                
                break;
        }

        PID_instance[i] -> set_dt(sampling_time);
    } 
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
    // Calculate OUTPUT
    for(int i = 0 ; i < MOTOR_NUM ; i++)
    {
        int temp_output = 0;
        for(int j = 0 ; j < PID_NUM ; j++)
        {   
            switch (PID_instance[j]->get_num())
            {
                case 0: //YAW
                    temp_output += (motor_instance[i]->get_axis()[2] ? PID_instance[j]->get_PID() : - PID_instance[j]->get_PID());
                    break;
            
                case 1: //PITCH
                    temp_output += (motor_instance[i]->get_axis()[1] ? PID_instance[j]->get_PID() : - PID_instance[j]->get_PID());
                    break;

                case 2: //ROLL
                    temp_output += (motor_instance[i]->get_axis()[0] ? PID_instance[j]->get_PID() : - PID_instance[j]->get_PID());
                    break;

                case 3: //X
                    temp_output += (motor_instance[i]->get_axis()[1] ? PID_instance[j]->get_PID() : - PID_instance[j]->get_PID());
                    break;

                case 4: //Y
                    temp_output += (motor_instance[i]->get_axis()[0] ? PID_instance[j]->get_PID() : - PID_instance[j]->get_PID());
                    break;

                case 5: //Z
                    temp_output += PID_instance[j]->get_PID();
                    break;
            }
        }

        motor_instance[i]->set_output(thrust + temp_output);
    }
}

void control::ctrl()
{
    get_sensor();
    set_current();
    cal_sampling_time();
    cal_PID();
    cal_Output();
}
