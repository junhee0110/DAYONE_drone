#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "sensor.h"
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

balance gy86;

void setup() {
  // put your setup code here, to run once:
  gy86.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  gy86.get_state();
  Serial.println(gy86.get_alt());
    
}
