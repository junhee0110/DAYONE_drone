#include "config.h"
#include "communication.h"



void Communication::init(){
  radio.begin();  
  }

void Communication::ReceiveSendMode(String strMode){
  if(strMode == "receive"){
    Serial.println("RECEIVE MODE");
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    }
   else if(strMode == "send"){
   Serial.println("SEND MODE");
   radio.openWritingPipe(address); 
   radio.setPALevel(RF24_PA_MIN); 
   radio.stopListening(); 
  }
  else if(strMode == "receiveACK"){
    Serial.println("RECEIVE ACK");
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    }
  else if(strMode == "sendACK"){
   Serial.println("SEND ACK"); 
   radio.openWritingPipe(address); 
   radio.setPALevel(RF24_PA_MIN); 
   radio.stopListening();
    } 
  }
  
void Communication::ReceiveDataSend(char strSendData[]){
  if(SetMode == "receive"){
    if (radio.available()) {  
      char data[32]="";
    for(int i=0;i<intReSendCnt;i++){
      delay(70);radio.read(&data, sizeof(data));
      String DATA = data; Serial.println(DATA);
      }
    char text[32] = "";
    radio.read(&text, sizeof(text));
    String strReceiveData = text;
    Serial.print("Receive Data: "); Serial.println(strReceiveData);
    String strComm = strReceiveData.substring(0,4);
    if(strComm == "send"){
      ReceiveSendMode("sendACK");  
      char text1[32] = "ACKS";
      radio.write(&text1,sizeof(text1)); 
      ReceiveSendMode("receiveACK");
      char text2[32]="";
      while(radio.available()==0){;}
      radio.read(&text2,sizeof(text2));
     String strData=text2; Serial.println(strData);
     if(strData == "ACKR"){
        SetMode="send";
        ReceiveSendMode("send");
      }
     }
    }
  }
  else if(SetMode == "send"){
    char text11[100]="";
    strcpy(text11,strSendData);
    //strcpy(text11,strSendData);
    for(int i = 0; i<intReSendCnt;i++){
      delay(70);radio.write(&text11,sizeof(text11));
      Serial.println(text11);
      }   
    const char text[] = "send change";//send mode로 바꿔라
    radio.write(&text,sizeof(text)); 
    Serial.println("Send Commend: send OK"); 
    char text2[32]="";
    ReceiveSendMode("receiveACK");
    while(radio.available()==0){}
    radio.read(&text2,sizeof(text2));
    String strData=text2; Serial.println(strData);
    if(strData=="ACKS"){
       ReceiveSendMode("sendACK");
       const char text1[] = "ACKR";
       radio.write(&text1,sizeof(text1));
       SetMode="receive"; 
       ReceiveSendMode("receive");
     }           
  }
  }

String Communication::ReadSerial(){
  String str = "";
  char ch;
  while(Serial.available()>0){
    ch = Serial.read();
    str.concat(ch);
    delay(10);
    }
    return str;
  }

void Communication::Send(int Prodata[]){                 
  byte PCdata[9]={0};
  for(int i=0;i<9;i++){
    PCdata[i]=Prodata[i];
    }
  Serial.write(PCdata,9); 
  }     
