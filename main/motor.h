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

    public:
        motor(int pin);
        void init();
        void rotate(int value);
};

//range of writeSignal : 700~2000
#endif
