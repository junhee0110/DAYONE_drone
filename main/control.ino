#include "config.h"
#include "control.h"

//------------------------------------------ㅡMOTOR_CONTROL-------------------------------------

motor_control::motor_control(int pin_num, bool dir) {
  this -> pin_num = pin_num;
  this -> dir = dir;
}

void motor_control::cal_output(axis &one, axis &two) {
  this -> output_val += (one.cal_PID() + two.cal_PID());
}

void motor_control::cal_output(axis &one, axis &two, axis &three) {
  this -> output_val += (one.cal_PID() + two.cal_PID() + three.cal_PID());
}
void motor_control::cal_output(axis &one, axis &two, axis &three, axis &four) {
  this -> output_val += (one.cal_PID() + two.cal_PID() + three.cal_PID() + four.cal_PID());
}

void motor_control::output() {
//servo라이브러리로 BLDC모터 제어
}
