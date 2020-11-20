#ifndef COMMU
#define COMMU

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
#endif

/* RX 부분
#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);     // SPI통신을 위한 (CE, CSN) 핀 선언
const byte address[6] = "00001"; // 송신기와 수신기가 동일한 값으로 주소 설정함(5자리)
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);// 데이터를 받을 송신기 주소를 설정
  radio.setPALevel(RF24_PA_MIN); // 송신거리에 따른, 전원공급 파워레벨 설정
//(최소) RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX (최대) 설정가능
//송신이 약하다고 판단될 경우 nRF24모듈의 GND와 3.3V 단자에 전해콘덴서(10uF이상:+를3.3V연결)사용권장
  radio.startListening();   // 모듈을 수신기(상태)로 설정
}
void loop() {
  if (radio.available()) {
    char text[32] = "";   // 데이터를 수신 받을 변수 설정
    radio.read(&text, sizeof(text)); // 수신되는 데이터 길이만큼 읽어 저장
    Serial.println(text);
  }
}
 */

/* TX 부분
 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // SPI통신을 위한 (CE, CSN) 핀 선언
const byte address[6] = "00001"; // 송신기와 수신기가 동일한 값으로 주소 설정함(5자리)
void setup() {
  radio.begin();
  radio.openWritingPipe(address); // 데이터를 보낼 수신 주소를 설정
  radio.setPALevel(RF24_PA_MIN); // 송신거리에 따른, 전원공급 파워레벨 설정
//(최소) RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX (최대) 설정가능
//송신이 약하다고 판단될 경우 nRF24모듈의 GND와 3.3V 단자에 전해콘덴서(10uF이상:+를3.3V연결)사용권장

  radio.stopListening();  // 모듈을 송신기로 설정
}
void loop() {
  const char text[] = "(From) nRF24 Tx : Hello World!"; // 송신할 문자
  radio.write(&text, sizeof(text)); // 위 문자를 문자 길이 만큼 송출함
  delay(1000);
}  
 */ 
