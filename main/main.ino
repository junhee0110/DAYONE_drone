#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "motor.h"
#include "sensor.h"
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

gy86 GY86;
motor m1(5,0);

void setup() {
  // put your setup code here, to run once:
  m1.init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val1=map(analogRead(A0),0,1023,700,2000);
  m1.set_output(val1);
  m1.rotate();
  Serial.println(val1);
}
