// File name: motor.h
// Purpose: control motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-09

#ifndef MOTOR
#define MOTOR

#include "Servo.h"

class motor: public Servo{
    private:
        int output;
        int pinNum;
        int motor_num;
        bool x_axis, y_axis, z_axis;
        int output;

    public:
        motor(int pin, int motor_num);
        void init();
        void rotate();
        void set_output(int val);
        bool * get_axis();

};

//range of writeSignal : 700~2000
//Motor Number Diagram
// ^ y 
//  1(01)   0(00)  > x(front)
//  2(10)   3(11)
#endif