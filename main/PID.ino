#include "PID.h"

axis::axis(float kP, float kI, float kD) {
  this -> kP = kP;
  this -> kI = kI;
  this -> kD = kD;
}

void axis::cal_err(float target, float current) {
  this -> e_tmp = this -> err;
  this -> err = target - current;

}

#ifdef FLEX_T
void axis::cal_dt() {
  float t = millis() / 1000;

  this -> dt = t - (this -> t_tmp);
  this -> t_tmp = t
}
#endif

#ifdef FIX_T
void axis::cal_dt() {
  this -> dt = FIX_T / 1000
  }
#endif

void axis::cal_inte() {
  this -> inte = (this -> err) * (this -> dt);

}

void axis::cal_diff() {
  this -> diff  = ((this -> err) - (this -> e_tmp)) / (this -> dt);

}

void axis::cal_PID() {
  this-> PID_val = (this->kP) * (this->err) + (this->kI) * (this->inte) + (this->kD) * (this->diff);

}

float axis::get_PID(){
  return this->PID_val;
}

float axis::get_kP(){
  return this->kP;
}

float axis::get_kI(){
  return this->kI;
}

float axis::get_kD(){
  return this->kD;
}

float axis::get_err(){
  return this->err;
}

void axis::set_const(float kP, float kI, float kD){
  this -> kP = kP;
  this -> kI = kI;
  this -> kD = kD;
}
