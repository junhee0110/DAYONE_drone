// File Name : PID.ino
// Purpose : defines class PID
// last edit : 2021-02-13
// Author : Hwang Tae-on, Lee Junhee
//------------- Edit log --------------------
// 21-02-08 First Edition of double loop PID  (Tae-on)
// 21-02-13 Second Edition of double loop PID (Junhee)

#include "PID.h"
#include "config.h"

PID::PID(int num, float kP1 = 1.0f, float kP2 = 1.0f, float kI = 1.0f, float kD = 1.0f) // Constructor
{ 
  //Initialize PID Constants
  this -> kP1 = kP1;
  this -> kP2 = kP2;
  this -> kI = kI;
  this -> kD = kD;
  number = num;

  //Initialize previous rate error
  rate_err_tmp = 0.0f;
}

void PID::set_first_target(float angle_target) // Set angle_target (From communication)
{
  this -> angle_target = angle_target;
}

void PID::set_first_current(float angle_current) // Set angle_current (From sensor)
{
  this -> angle_current = angle_current;
}

void PID::set_rate_target(float rate_target) // Set rate_target (From sensor)
{
  this -> rate_target = rate_target;
}

void PID::set_const(float kP1, float kP2, float kI, float kD) // Set PID Constants (From communication)
{
  this -> kP1 = kP1;
  this -> kP2 = kP2;
  this -> kI = kI;
  this -> kD = kD;
}

void PID::set_dt(float dt) // Set Sampling time (From control)
{
  this -> dt = dt;
}

void PID::cal_angle_err() // Calculate angle error
{
  angle_err = angle_target - angle_current;
}

void PID::P_angle() // Calculate P value using actual angle
{
  P_first = angle_err * kP1;
}

void PID::cal_rate_err() // Calculate Error of Rate
{
  rate_err_tmp = rate_err;
  rate_err = rate_target - P_first;
}

void PID::cal_PID() // Calculate PID value
{
  I_val += rate_err * dt;

  PID_val = kP2 * rate_err + kI * I_val + kD * ((rate_err - rate_err_tmp) / dt);
}

float PID::get_PID() // get PID value (to control)
{
  return PID_val;
}

float PID::get_kP1() //get kP1 value (to communication)
{
  return kP1;
}

float PID::get_kP2() //get kP2 value (to communication)
{
  return kP2;
}

float PID::get_kI() //get kI value (to communication)
{
  return kI;
}

float PID::get_kD() //get kD value (to communication)
{
  return kD;
}

int PID::get_num()
{
  return number;
}
