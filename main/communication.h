#ifndef COMMU
#define COMMU
#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8);     // SPI통신을 위한 (CE, CSN) 핀 선언
//String SetMode= "receive";

class Communication {
  public:
    void init();
    void ReceiveSendMode(String strMode);
    void ReceiveDataSend(char strSendData[]);
    String ReadSerial(); 
    void Send(int prodata[]); //프로세싱 데이터
    int prodata[9];
    String SerialInStr = "";
    int intReSendCnt = 1;
    byte address[6] = "00001";       
  };    
#endif
