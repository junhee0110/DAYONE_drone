#include "PID.h"
#include "config.h"

//------------------------------------------ㅡAXIS-------------------------------------

axis::axis(float kP, float kI, float kD) {
  this -> kP = kP;
  this -> kI = kI;
  this -> kD = kD;
}

float axis::cal_err(float target, float current) {
  this -> e_tmp = this -> err;
  this -> err = target - current;

}

#ifdef FLEX_T
float axis::cal_dt() {
  float t = millis() / 1000;

  this -> dt = t - (this -> t_tmp);
  this -> t_tmp = t;

  return this -> dt;
}
#endif

#ifdef FIX_T
float axis::cal_dt() {
  this -> dt = FIX_T / 1000

  return this -> dt;
  }
#endif

float axis::cal_inte() {
  this -> inte = (this -> err) * (this -> dt);
  return this -> inte;
}

float axis::cal_diff() {
  this -> diff  = ((this -> err) - (this -> e_tmp)) / (this -> dt);
  return this -> diff;
}

float axis::cal_PID() {
  this-> PID_val = (this->kP) * (this->cal_err(this -> target, this-> current)) + (this->kI) * (this->cal_inte()) + (this->kD) * (this->cal_diff());
  return this -> PID_val;
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
