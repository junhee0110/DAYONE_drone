#include "config.h"
#include "differ_integ.h"

float dt = 0.0;
float inte= 0.0;
float diff = 0.0;

#ifdef FLEX_T
float t1 = 0.0;
float t2;

float get_dt(){
  t2 = millis()/1000;

  if (t2<=t1)
    return t2;

  else
    return t2-t1;
}
#endif

#ifdef FIX_T

float get_dt(){

  return FIX_T/1000;
}

#endif

float get_err(float desired, float current){
  return desired - current;
}

float get_inte(float err){
  return inte + err*dt;
}

float get_diff(float err){
  return err/dt;
}
