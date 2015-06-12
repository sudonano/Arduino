#include <SPI.h>
#include <RF24.h>
//#include <nRF24L01.h>
//#include "printf.h"
//#include <RF24_config.h>
//#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>

// Radio Hardware Configuration
RF24 radio(9, 10); // Set up nRF24L01 radio on SPI bus plus pins 9 & 10

// Radio Topology
const uint64_t pipes[2] = { 0xE8E8F0F0E1LL, 0xF0F0F0F0D2LL }; // Radio pipe addresses for the 2 nodes to communicate.

// Role management
typedef enum { role_ping_out = 1, role_pong_back } role_e;                // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"}; // The debug-friendly names of those roles
role_e role = role_ping_out;                                              // The role of the current running sketch

const int buttonPin = 3;
const int pingPin = 4;
const int ledPin = 5;
const int sensorMode = 1;
int ledState = HIGH;
int buttonState;
int lastButtonState;
long lastDebounceTime = 0;
long debounceDelay = 50;
unsigned long lastPing = 0;
unsigned long pingTime = millis();
long randPick = 0;
//int msg[1];
bool ok;
int mowCase = 0;
String subState = "PARKED(";
String subOne = "h";
String subTwo = "m";
String subThree = "s)";
int dispHours = 0;
int lastPingRet;

typedef struct{
  char staticMessage[16] = "M-B STATUS V1.0";
  bool isParked = 0;
  unsigned long mowTime = 0;
  unsigned long parkedTime = 86390;
}robInfo_t;

robInfo_t robInfo;

void setup(void)
{
  Serial.begin(57600);
  Serial.println("Node Started - Entering transmit mode.");  
//  printf_begin();

  // Setup and configure rf radio
  radio.begin();
  radio.setRetries(15, 15);
//  rf24_pa_dbm_e radioPA = radio.getPALevel();
//  Serial.print("PA level: ");Serial.println(radioPA);
//  radio.setPALevel(RF24_PA_MAX);
//  radioPA = radio.getPALevel();
//  Serial.print("PA level: ");Serial.println(radioPA);
  radio.setDataRate(RF24_250KBPS);
  Serial.println("Set datarate to 250KBPS");
  
  
  
  // Open pipes to other nodes for communication
  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
  }

  // Start listening
  //  radio.startListening();

  // Dump the configuration of the rf unit for debugging
  //radio.printDetails();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  ledState = LOW;
  digitalWrite(ledPin, ledState);
}

void loop(void) {

//  if (pingTime == 0)
    pingTime = millis();

  if (((pingTime - lastPing) > 1000) && role == role_ping_out ){
    String robotStatus;
      
    if ( sensorMode == 0){
        
      int buttonState = digitalRead(buttonPin);

      
      if (buttonState == 1 && lastButtonState == 1){
        robInfo.isParked = 1;
        robInfo.parkedTime = robInfo.parkedTime + ((millis() - pingTime)/1000);
      }
      else if (buttonState == 0 && lastButtonState == 0){
        robInfo.isParked = 0;
        robInfo.mowTime = robInfo.mowTime + ((millis() - pingTime)/1000) ;
      }
      
      lastButtonState = buttonState;
    }
    
    else{
     
      int pingRet = pinger();

      
      if (pingRet >= 15 && lastPingRet >= 15){
        robInfo.isParked = 0;
//        robInfo.mowTime = robInfo.mowTime + ((millis() - pingTime)/1000);
        robInfo.mowTime = robInfo.mowTime + 1;
        //delay the reset so that the Server can catch the total mow time
        if (robInfo.mowTime >= 5)
          robInfo.parkedTime = 0;
      }
      else if (pingRet < 15 && lastPingRet < 15){
        robInfo.isParked = 1;
        robInfo.parkedTime = robInfo.parkedTime + 1;
//        robInfo.parkedTime = robInfo.parkedTime + ((millis() - pingTime)/1000);
        //delay the reset
        if (robInfo.parkedTime >= 5)
        robInfo.mowTime = 0;
      }
      
      lastPingRet = pingRet;
      Serial.print("pingRet = ");Serial.println(pingRet);
      Serial.print("LastPingRet = ");Serial.println(lastPingRet);
    }
        
    

    
//    String messageToSend = staticMessage + robotStatus + parkedTime + mowTime;
//    int messageSize = messageToSend.length();
//    Serial.println(messageToSend);


//    for (int i = 0; i < messageSize; i++) {
//      int charToSend[1];
//      charToSend[0] = messageToSend.charAt(i);
//      ok = radio.write(charToSend, 1);
//    }
//    
//    //ok = radio.write( &mowTime, sizeof(int) );
//    //ok = radio.write( &parkedTime, sizeof(int) );
//
//    //send the 'terminate string' value...
//    msg[0] = 2;
//    radio.write(msg, 1);

  
  radio.stopListening();
  radio.setPayloadSize(sizeof(robInfo));
  Serial.print(sizeof(robInfo));
  bool ok = radio.write( &robInfo, sizeof(robInfo) );
  //radio.startListening();

  Serial.write("BOOL ok result: "); Serial.println(ok);
  Serial.println("Package contents: ");
  Serial.print("\t Static message = ");
  Serial.println(robInfo.staticMessage);
  Serial.print("\t isParked = ");
  Serial.println(robInfo.isParked);
  Serial.print("\t parkedTime = ");
  Serial.println(robInfo.parkedTime);
  Serial.print("\t mowTime = ");
  Serial.println(robInfo.mowTime);
  
  if (ok) {
    ledState = HIGH;
  }
  else
  {
    ledState = LOW;
  }
  
  digitalWrite(ledPin, ledState);
  lastPing = pingTime;
  

  //  else if (role == pong_back_role)
  //  {
  //  }

  //switchMode();
  // buttonRead();
  }

}
