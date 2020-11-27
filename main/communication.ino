#include "config.h"
#include "communication.h"



void ComRX::init(){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);// 데이터를 받을 송신기 주소를 설정
  radio.setPALevel(RF24_PA_MIN); // 송신거리에 따른, 전원공급 파워레벨 설정
  //(최소) RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX (최대) 설정가능
  //송신이 약하다고 판단될 경우 nRF24모듈의 GND와 3.3V 단자에 전해콘덴서(10uF이상:+를3.3V연결)사용권장
  radio.startListening();   // 모듈을 수신기(상태)로 설정
  }

void ComRX::RX(){
  if (radio.available()) {
    radio.read(&rxdata, sizeof(rxdata)); // 수신되는 데이터 길이만큼 읽어 저장
    Serial.println(rxdata); Serial.println(" 수신 ");
  }
  else {
    Serial.println("통신모듈 오류");
    }
  }


void ComTX::init(){
  radio.begin();
  radio.openWritingPipe(address); // 데이터를 보낼 수신 주소를 설정
  radio.setPALevel(RF24_PA_MIN); // 송신거리에 따른, 전원공급 파워레벨 설정
  radio.stopListening();  // 모듈을 송신기로 설정
  }

void ComTX::TX(char txdata[]){   
  radio.write(&txdata, sizeof(txdata)); // 위 문자를 문자 길이 만큼 송출함
  Serial.print(txdata);Serial.println(" 전송 ");
  }  

void ProSend::Send(int Prodata[]){                 
  byte PCdata[9]={0};
  for(int i=0;i<9;i++){
    PCdata[i]=Prodata[i];
    }
  Serial.write(PCdata,9); 
  }     
