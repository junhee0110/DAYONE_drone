// File name: motor.ino
// Purpose: control motor
// Author: Lee Junhee, Hong Miru
// Last Edit: 20-02-09

#include "motor.h"

motor::motor(int pin, int motor_num){
    pinNum = pin;
    this -> motor_num = motor_num;
    z_axis = (bool)(motor_num % 2);
    x_axis = (bool)((motor_num - z_axis) / 2);
    y_axis = !((x_axis || z_axis) && !(x_axis && z_axis));

}

void motor::init(){
    Servo::attach(pinNum);
}

void motor::rotate(int value){
    Servo::writeMicroseconds(value);
}
