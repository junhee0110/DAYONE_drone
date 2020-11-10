#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "sensor.h"

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
