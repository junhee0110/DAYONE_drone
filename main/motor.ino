// File name: motor.ino
// Purpose: control motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-09

#include "motor.h"

motor::motor(int pin){
    pinNum = pin;
}

void motor::init(){
    Servo::attach(pinNum);
}

void motor::rotate(int value){
    Servo::writeMicroseconds(value);
}
