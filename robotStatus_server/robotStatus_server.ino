#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// LCD Hardware Configuration
#define I2C_ADDR    0x27 // Hanks I2C ADDRESS IS 0x27
#define BACKLIGHT_PIN   3
#define En_pin          2
#define Rw_pin          1
#define Rs_pin          0
#define D4_pin          4
#define D5_pin          5
#define D6_pin          6
#define D7_pin          7

//LCD variables
int lcdLength = 16;
int parts;
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin); //Set LCD pinout

/*
This sketch receives strings from sending unit via nrf24 
and prints them out via serial.  The sketch waits until
it receives a specific value (2 in this case), then it 
prints the complete message and clears the message buffer.
*/
 
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int last_isParked = 0;
unsigned long dispCount;
unsigned long dispDays;
unsigned long dispHours;
unsigned long dispMin;
unsigned long dispSec;
String subState;
String subBrack = "";
String subD = "d";
String subOne = "h";
String subTwo = "m";
String subThree = "s       ";
String subBlank = "        ";
String lineOne;
int mowCycles = 0;
int caseNr;

/** VARIABLES FOR SERVER SIDE STRING BUILDING **/
typedef struct{
  char staticMessage[16];
  bool isParked = 0;
  unsigned long mowTime = 0;
  unsigned long parkedTime = 0;
}robInfo_t;

robInfo_t robInfo;

void setup(void){
  
  //LCD Setup
  lcd.begin (16,2); //LCD size
  Serial.begin(57600);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE); // Switch on the backlight
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MARK-BOERJE");
  lcd.setCursor(0,1);
  lcd.print("STATUS V1.1");
  lcd.print("               ");
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(sizeof(robInfo));
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  delay(3000);
  
}
void loop(void){
   
  String lineTwo;
  if (radio.available()){
    bool done = false;
    while (!done)
    {
      done = radio.read( &robInfo, sizeof(robInfo) );
    }
    
//    Serial.println("Package contents: ");
//    Serial.print("\t Static message = ");
//    Serial.println(robInfo.staticMessage);
//    Serial.print("\t isParked = ");
//    Serial.println(robInfo.isParked);
//    Serial.print("\t parkedTime = ");
//    Serial.println(robInfo.parkedTime);
//    Serial.print("\t mowTime = ");
//    Serial.println(robInfo.mowTime);
  
    if (robInfo.isParked == 1 && last_isParked == 1){
      subState = "PARKED ";
      dispCount = robInfo.parkedTime;
      dispDays = dispCount / 86400;
      int x = dispDays * 24;
      dispHours = (dispCount / 3600) - x; //(dispDays * 24);
      dispMin = (dispCount/60) - (dispDays * 24 * 60) - (dispHours * 60);
      dispSec = dispCount - (dispDays * 24 * 3600) - (dispHours * 3600) - (dispMin * 60);
 
      Serial.print("dispCount: ");Serial.println(dispCount);
      Serial.print("Days: ");Serial.println(dispDays);
      Serial.print("Hours: ");Serial.println(dispHours);
      Serial.print("Minutes: ");Serial.println(dispMin);
      Serial.print("Seconds: ");Serial.println(dispSec);      
      if (robInfo.parkedTime < 60){
        lineOne = subState + subBrack + dispSec + subThree + subBlank;        
      }
      else if (robInfo.parkedTime >=60 && robInfo.parkedTime < 3600){
        lineOne = subState + subBrack + dispMin + subTwo + dispSec + subThree + subBlank;   
      }
      else if (robInfo.parkedTime >=3600 && robInfo.parkedTime < 86400){
        lineOne = subState + subBrack + dispHours + subOne + dispMin + subTwo + dispSec + subThree + subBlank;        
      }
      else {
        lineOne = subState + subBrack + dispDays + subD + dispHours + subOne + dispMin + subTwo + subBlank;
      }
    }
    else if (robInfo.isParked == 0 && last_isParked == 0){
      subState = "MOWING ";
      dispCount = robInfo.mowTime;
      dispDays = dispCount / 86400;
      int x = dispDays * 24;
      dispHours = (dispCount / 3600) - x; //(dispDays * 24);
      dispMin = (dispCount/60) - (dispDays * 24 * 60) - (dispHours * 60);
      dispSec = dispCount - (dispDays * 24 * 3600) - (dispHours * 3600) - (dispMin * 60);     
  
      if (robInfo.mowTime < 60){
        lineOne = subState + subBrack + dispSec + subThree + subBlank;        
      }
      else if (robInfo.mowTime >=60 && robInfo.mowTime < 3600){
        lineOne = subState + subBrack + dispMin + subTwo + dispSec + subThree + subBlank;   
      }
      else if (robInfo.mowTime >=3600 && robInfo.mowTime < 86400){
        lineOne = subState + subBrack + dispHours + subOne + dispMin + subTwo + dispSec + subThree + subBlank;        
      }
      else {
        lineOne = subState + subBrack + dispDays + subD + dispHours + subOne + dispMin + subTwo + subBlank;
      }
    }
    else if (robInfo.isParked == 0 && last_isParked == 1){
      //Serial.print("Parked time: ");Serial.print(robInfo.parkedTime);
    }
    else if (robInfo.isParked == 1 && last_isParked == 0){
      //Serial.print("Mowing cycletime ");Serial.print(robInfo.mowTime);
      mowCycles = mowCycles + 1;
    }
    //Serial.println("Time variables -");
    //Serial.print("robInfo.parkedTime: ");Serial.println(robInfo.parkedTime);
    //Serial.print("dispCount (int): ");Serial.println(dispCount);
    //Serial.print("dispHours: ");Serial.println(dispHours);
    //Serial.print("dispMin: ");Serial.println(dispMin);
    //Serial.print("dispSec: ");Serial.println(dispSec);
      
//    else if (robInfo.isParked == 1 && last_isParked == 0){
//      subState = "Just Parked";
//      lineOne = subState + robInfo.parkedTime + subThree;
//      }
  
    lcd.setCursor(0,0);
      lcd.print(lineOne);
    lcd.setCursor(0,1);
        lcd.print(lineTwo);
//      lcd.print("Cycles: ");
//      lcd.print(mowCycles);
//      lcd.print("    ");
    
    Serial.println(robInfo.isParked);
    }


   
      
//      if (robInfo.isParked == 1){
//        lcd.print(lineOne);
//        lcd.print("    ");
//      }
//      else {
//        lcd.print("Mowing: ");
//        lcd.print(robInfo.mowTime);
//        lcd.print("    ");
//      }
          
  last_isParked = robInfo.isParked;
} 



//        lcd.setCursor(blankCursor,1);
//          for (int i = 0; i < blankSpaces; i++){
//            lcd.print(" ");
//          }

//    if (buttonState == 1 && lastButtonState == 1) {
//      
//
//      dispHours = parkedTime / 3600;
//      int dispMin = (parkedTime - dispHours * 60) / 60;
//      int dispSec = parkedTime - dispMin * 60;
//
//      if (parkedTime < 60){
//        robotStatus = subState + dispSec + subThree;        
//      }
//      else if (parkedTime >=60 && parkedTime < 3600){
//        robotStatus = subState + dispMin + subTwo + dispSec + subThree;   
//      }     
//      else {
//        robotStatus = subState + dispHours + subOne + dispMin + subTwo + dispSec + subThree;        
//      }
// 
//      parkedTime = parkedTime + 1;     
//    }
//    
//    else if (buttonState == 0 && lastButtonState == 1) {
//      robotStatus = "JUST LEFT";
//      mowTime = 0;
//
//    }
//    else if (buttonState == 1 && lastButtonState == 0) {
//      robotStatus = "JUST PARKED";
//      parkedTime = 0;
//      
//    }
//    else {
//      String subOne;
//      
//      switch (mowCase) {
//        case 0:
//          subOne = " MOWING (";
//          mowCase = 1;
//          break;
//        case 1:
//          subOne = "#MOWING#(";
//          mowCase = 0;
//          break;
//      }
//      
//      mowTime = mowTime + 1;
//      String subTwo = "m.";
//      String subThree = "s.)";
//      int dispMin = mowTime / 60;
//      int dispSec = mowTime - dispMin * 60; 
//
//      if (mowTime < 60){
//        robotStatus = subOne + dispSec + subThree;        
//      }
//      else {
//        robotStatus = subOne + dispMin + subTwo + dispSec + subThree;        
//         }
//         
//
//    }
