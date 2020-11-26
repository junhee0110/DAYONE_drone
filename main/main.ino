#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "gy86.h"
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

gy86 gy86;

void setup() {
  // put your setup code here, to run once:
  gy86.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  gy86.get_state();
  Serial.print("yaw\t");
  Serial.print(gy86.get_yaw());
  Serial.print("pitch\t");
  Serial.print(gy86.get_pitch());
  Serial.print("roll\t");
  Serial.print(gy86.get_roll());
  Serial.print("alt\t");
  Serial.print(gy86.get_alt());
  Serial.print("head\t");
  Serial.print(gy86.get_head());


    
}
