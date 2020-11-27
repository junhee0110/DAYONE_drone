#ifndef COMMU
#define COMMU
#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8);     // SPI통신을 위한 (CE, CSN) 핀 선언

class ComRX {

  public:
    const byte address[6] = "00001"; // 송신기와 수신기가 동일한 값으로 주소 설정함(5자리)
    char rxdata[32]="";
    void init();
    void RX();
  };

class ComTX{

  public:
    const byte address[6] = "00001";
    char txdata[32] = "(From) nRF24 Tx : Hello World!"; // 송신할 문자
    void init();
    void TX(char txdata[]);   
   };

class ProSend{
  
  public:
    int prodata[9];
    void Send(int prodata[]);  
  };     
#endif
