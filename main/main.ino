#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "motor.h"
#include "sensor.h"
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

PID * PID_array[] = {new PID(0), new PID(1), new PID(2)};
motor * motor_array[] = {new motor(5, 0), new motor(6, 1), new motor(9, 2), new motor(10, 3)};

control ctrl(PID_array, motor_array);

void setup() {
  // put your setup code here, to run once:
  ctrl.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  ctrl.ctrl();
}
