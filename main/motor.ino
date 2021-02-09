// File name : motor.ino
// Last Update : 2021-02-09
// Purpose : Control Motor
// Author : Hong Miru
//------------- Edit log --------------------

#include "config.h"
#include "control.h"
#include <Servo.h> //라이브러리 불러오기
Servo bldc ; //모터 선언
Serial.begin(9600) ;
bldc.attach(9);
//------------------------------------------ㅡMOTOR_CONTROL-------------------------------------

motor_control::motor_control(int pin, int motor){
  int temp = motor_num;
  pin_num = pin_num;
  motor_num = motor_num;
  x_sign = (bool)(temp%2);
  y_sign = (bool)((temp-(temp%2))/2);
}

void motor_control::cal_output(axis &x, axis &y) {
  this -> output_val += ( (this-> x_sign ? -1 * x.cal_PID() : x.cal_PID() ) + (this-> y_sign ? -1 * y.cal_PID() : y.cal_PID() ) );
}

void motor_control::output() {
//servo라이브러리로 BLDC모터 제어

  bldc.write(pos);
  Serial.println(pos);
}
