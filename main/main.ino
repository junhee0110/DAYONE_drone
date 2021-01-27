#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "sensor.h"

balance gy86;
Com CUM;

void setup() {
  // put your setup code here, to run once:
 // gy86.init();
 Serial.begin(9600);
 //CUM.initrx();
 CUM.inittx();

}

void loop() {
  //gy86.get_state();
  //Serial.println(gy86.get_alt());
 char txdata[32] = "Hello World!"; // 송신할 문자 
 //int data[9]={1,2,3,4,5,6,7,8,9};
 //CUM.Send(data);
 CUM.TX(txdata);
 //CUM.RX();
  delay(1300);    
}
