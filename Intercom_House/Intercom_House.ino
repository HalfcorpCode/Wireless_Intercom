#include <PS2Keyboard.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

LiquidCrystal LCD(4, 5, 6, 7, 8, 9);
const int DataPin = 3;
const int IRQpin =  2;

int CE_PIN = A0;
int CSN_PIN = 10;
const byte LabAddress[5] = {'R','x','A','A','A'};
const byte LocalAddress[5] = {'R','x','B','B','B'};
RF24 radio(CE_PIN, CSN_PIN);
char Data1[16] = "";
char Data2[16] = "";
char Data3[16] = "";
char Data4[16] = "";

char dataReceived[16];
char DataIn1[16] = "";

int Position = 0;
int Page = 0;
char Payload[16][4];

PS2Keyboard keyboard;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");

  LCD.begin(16, 2);
  LCD.cursor();

  Initialize_Payload();
  Print_Payload();

  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3,5); // delay, count
  radio.openWritingPipe(LabAddress);

  radio.openReadingPipe(1, LocalAddress);
  radio.startListening();

}

void loop() {

  getData();
  
  if (keyboard.available()) {
    // read the next key
    char c = keyboard.read();
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Send_Message();
    } else if (c == PS2_ESC) {
      LCD.noDisplay();
      delay(100);
      LCD.display();  
      
      LCD.clear();
      Position = 0;
      Page = 0;
      Initialize_Payload();
      Print_Payload();
      LCD.setCursor(0, 0);
    } 
    else if (c == '#'){
    LCD.print("PING");
    delay(1000);
    LCD.clear();
    Position = 0;
    Page = 0;
    Initialize_Payload();
    Payload[0][0]= '#';
    Send_Message();
    }
    
    else if (c == PS2_DELETE) {

    if (Position == 0 && Page == 0)
    {}
    else{

      if (Position == 0){
      Page--;
      LCD.clear();

      for (int i=0; i<16; i++){
        LCD.print(Payload[i][Page]);
      }
      
      LCD.setCursor(15, 0);
      Position = 16;
      }
      
      Position --;
      LCD.setCursor(Position, 0);
      LCD.print(" ");
      LCD.setCursor(Position, 0);
      Payload[Position][Page] = '*';
      Print_Payload();
    }
      
    } else {

    if (Position == 16 && Page == 3)
    {}
    else{

      if (Position == 16){

    Page++;
    LCD.clear();
    LCD.setCursor(0, 0);
    Position = 0;
   
  }
      // otherwise, just print all normal characters
      LCD.print(c);
      Payload[Position][Page] = c;
      Print_Payload();
      Position ++;
      }
    }
  }
}

void Print_Payload(){

  int i = 0;
  int i2 = 0;

  for (i=0; i<4; i++){
    for (i2=0; i2<16; i2++){
      Serial.print(Payload[i2][i]);
    }
    Serial.print("\n");
  }
Serial.print("\n");
Serial.print("\n");
}

void Initialize_Payload(){

  int i = 0;
  int i2 = 0;

  for (i=0; i<4; i++){
    for (i2=0; i2<16; i2++){
      Payload[i2][i] = '*';
    }
  }
    
}

void Send_Message(){

  LCD.noDisplay();
  delay(100);
  LCD.display();

radio.stopListening();

  for (int i=0; i<16; i++){
      Data1[i] = Payload[i][0];
      Data2[i] = Payload[i][1];
      Data3[i] = Payload[i][2];
      Data4[i] = Payload[i][3];
  }

bool rslt;
    rslt = radio.write( &Data1, sizeof(Data1) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }

    
    rslt = radio.write( &Data2, sizeof(Data2) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }

  
    rslt = radio.write( &Data3, sizeof(Data3) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }

  
    rslt = radio.write( &Data4, sizeof(Data4) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }

    radio.startListening();

}
void getData() {
    if ( radio.available() ) {
        radio.read( &DataIn1, sizeof(DataIn1) );
        Serial.println(DataIn1);

        if(DataIn1[0] == '#'){
          LCD.print("PONG");
          delay(1000);
          LCD.clear();
          Position = 0;
          Page = 0;
          Initialize_Payload();
          Payload[0][0]= 'P';
          Payload[1][0]= 'O';
          Payload[2][0]= 'N';
          Payload[3][0]= 'G';
          Send_Message();
        }
        else{
        
        LCD.setCursor(0,1);
        LCD.print("                ");
        LCD.setCursor(0,1);
        LCD.print(DataIn1);
        LCD.setCursor(Position,0);
        }
    }
}


