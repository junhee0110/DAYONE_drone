#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "motor.h"
#include "sensor.h"
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

gy86 GY86;
motor m1 = motor(5);
motor m2 = motor(6);

void setup() {
  // put your setup code here, to run once:
  m1.init();
  m2.init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val1=map(analogRead(A0),0,1023,700,2000);
  int val2=map(analogRead(A1),0,1023,700,2000);
  m1.rotate(val1);
  m2.rotate(val2);
  Serial.print(val1);
  Serial.print("\t");
  Serial.println(val2);
}
