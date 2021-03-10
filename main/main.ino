#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "motor.h"
#include "sensor.h"
#include "pinNum.h"

PID * PID_array[] = {new PID(0), new PID(1), new PID(2)};
motor * motor_array[] = { new motor(MOTOR_ONE, 0)
                        , new motor(MOTOR_TWO, 1)
                        , new motor(MOTOR_THREE, 2)
                        , new motor(MOTOR_FOUR, 3)};

control ctrl(PID_array, motor_array);

void setup() {
  // put your setup code here, to run once:
  ctrl.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  ctrl.ctrl();
}
