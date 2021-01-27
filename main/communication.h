#ifndef COMMU
#define COMMU
#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8);     // SPI통신을 위한 (CE, CSN) 핀 선언


class Com {
  
  public:
    void initrx();
    void RX();
    void inittx();
    void TX(char txdata[]);
    int prodata[9];
    void Send(int prodata[]);
        
  };    
#endif
