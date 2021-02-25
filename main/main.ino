#include "config.h"
#include "PID.h"
#include "communication.h"
#include "control.h"
#include "sensor.h"

balance gy86;
Communication joystick;
String SetMode="send";

void setup() {
  // put your setup code here, to run once:
 // gy86.init();
 Serial.begin(9600);
 radio.begin();
 joystick.ReceiveSendMode(SetMode);
}

void loop() {
if(SetMode=="receive"){
  joystick.ReceiveSendMode(SetMode);
  joystick.ReceiveDataSend("");
 }
 else if(SetMode == "send"){
  joystick.SerialInStr=joystick.ReadSerial();
  if(joystick.SerialInStr != ""){
    char charSData[joystick.SerialInStr.length()+1];
    joystick.SerialInStr.toCharArray(charSData,joystick.SerialInStr.length());
    joystick.ReceiveSendMode(SetMode);
    joystick.ReceiveDataSend("WORLD");
    }  
  }
 delay(2000);   
}
