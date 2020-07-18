#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int CE_PIN = 9;
int CSN_PIN = 10;

const byte LocalAddress[5] = {'R','x','A','A','A'};
const byte HouseAddress[5] = {'R','x','B','B','B'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[16];
bool newData = false;
int PING = 0;
int Counter = 0;

String Input = "";
char Data1[16] = "";
char Data2[16] = "";
char Data3[16] = "";
char Data4[16] = "";

char dataToSend[16];

void setup() {

    Serial.begin(9600);
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, LocalAddress);
    radio.startListening();
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(HouseAddress);

}

void loop() {
    
    getData();
    showData();

    if(PING==1){
      if(Counter == 3){
      Send();
      PING = 0;
      Counter = 0;
      }
      else{
        Counter++;
      }
    }

    if (Serial.available() > 0) {
    Input = Serial.readString();
    Serial.println(Input);

  if(Input == "Test1"){

    analogWrite(3, 50);
    delay(500);
    analogWrite(3, 0);
        
  }
  else if(Input == "PING"){
    Data1[0] = '#';
    Serial.println("Pinging...");
    Send();
  }
  else{
    
  for (int i=0; i<16; i++){
      Data1[i] = Input[i];

  }
  for (int i =0; i<16; i++){
  Serial.print(Data1[i]);
  }
  
  Serial.println(Data1);

  Send();
    }
    }
}

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived);
        newData = false;
        analogWrite(3, 50);
        delay(500);
        analogWrite(3, 0);

        if(dataReceived[0]=='#'){
          Serial.println("Char confirmed");
          Data1[0] = 'P';
          Data1[1] = 'O';
          Data1[2] = 'N';
          Data1[3] = 'G';
          Serial.println(Data1);
          PING = 1;
          Counter++;
        }
    }
}

void Send() {

  radio.stopListening();
  
    bool rslt;
    rslt = radio.write( &Data1, sizeof(Data1) );

    Serial.print("Data Sent ");
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }
    radio.startListening();
}

