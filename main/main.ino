#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "sensor.h"
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

gy86 GY86;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  GY86.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  GY86.get_dmp();
  GY86.get_heading();
  Serial.print(GY86.get_yaw());
  Serial.print("\t");
  Serial.print(GY86.get_pitch());
  Serial.print("\t");
  Serial.print(GY86.get_roll());
  Serial.print("\t");
  Serial.println(GY86.get_head());
}
