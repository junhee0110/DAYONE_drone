#include "PID.h"
#include "config.h"

PID::PID(float kP1, float kP2, float kI, float kD)
{
  this -> kP1 = kP1;
  this -> kP2 = kP2;
  this -> kI = kI;
  this -> kD = kD;
}

void PID::set_angle_target(float angle_target)
{
  this -> angle_target = angle_target;
}

void PID::set_angle_current(float angle_current)
{
  this -> angle_current = angle_current;
}

void PID::set_rate_current(float rate_current)
{
  this -> rate_current = rate_current;
}

void PID::set_const(float kP1, float kP2, float kI, float kD)
{
  this -> kP1 = kP1;
  this -> kP2 = kP2;
  this -> kI = kI;
  this -> kD = kD;
}

void PID::set_dt(float dt)
{
  this -> dt = dt;
}

float PID::cal_angle_err()
{
  
}