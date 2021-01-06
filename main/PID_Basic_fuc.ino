#include "PID.h"
double Kp=a;
double KI=b;
double KD=c;
double target=x;
double current= //받아오는 값
double Time //루프 한번 돌 때 드는 시간

void pid_fuc(){
  double P,I,D;
  double err;
  double err_previous=0;
  err= target-current;
  P=Kp * err;
  I+=KI* err *Time
  D=KD* (err-err_previous)/Time
  all=P+I+D;
  err_previous=err;
  
}
