#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//DECLARE PINS FOR PING
unsigned long distance;
unsigned long dist;
unsigned long distChange;
unsigned long lastDist;
unsigned long lastPing = 0;
unsigned long pingTime = millis();
unsigned long lcdWrite = millis();
unsigned long lastLCDWrite = 0;
const int pingPin = 13;
const double maxDist = 400.00;
int lastParts = 0;
double lastDistRatio = 0;
double distRatio = 0;

//DECLARE FOR STEPPER

//DECLARE VARIABLES AND PINS FOR MOTORPINS
int motorPin1 = 8;    // Blue   - 28BYJ48 pin 1
int motorPin2 = 9;    // Pink   - 28BYJ48 pin 2
int motorPin3 = 10;    // Yellow - 28BYJ48 pin 3
int motorPin4 = 11;    // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 (VCC)
int motorSpeed = 2000;   //variable to set stepper speed
int count = 0;           // count of steps made
int steps = 0;           // count of pinion steps made = count/68
int direc = 1;           // 1 for CW 0 for CCW                        
int countsperrev = 512;  // number of steps per full revolution
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};



//DECLARE PINS FOR LCD
int lcdLength = 16;
int parts;
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin); //Set LCD pinout

byte char0[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
  };

byte char1[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
  };

byte char2[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
  };

byte char3[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
  };
byte char4[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
  };

byte char5[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
  };
  
void setup() {

  //SETUP STEPPER
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    Serial.begin(9600);
  //END SETUP STEPPER

  lcd.begin (16,2); //LCD size
  Serial.begin(9600);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE); // Switch on the backlight
  lcd.setBacklight(HIGH);
//  lcd.home (); // go home
  
  lcd.createChar(0, char0);
  lcd.createChar(1, char1);
  lcd.createChar(2, char2);
  lcd.createChar(3, char3);
  lcd.createChar(4, char4);
  lcd.createChar(5, char5);
}

void loop() {
  
  //PAN STEPPER

  if (steps >= 0 && steps <= 128 && direc == 1){
    clockwise();
    steps++;
  }
  else if (steps >= 0 && steps <= 128 && direc == 0){
    anticlockwise();
    steps--;
  }  
  else if (steps > 128){
    anticlockwise();
    steps--;
    direc = 0;
  }
  else if (steps < 0){
    clockwise();
    steps++;
    direc = 1;  
    }
    

  lcd.setCursor(0,0);
  

  distRatio = (dist/maxDist)*16.00;
  pingTime = millis();
  if (pingTime - lastPing > 100){
    dist = pinger();
      Serial.print("Distance: ");
      Serial.println(dist);
      Serial.print("distRatio: ");
      Serial.println(distRatio);
      Serial.print("lastDistRatio: ");
      Serial.println(lastDistRatio);
      lastPing = pingTime;
      Serial.print("lastPing: ");
      Serial.println(lastPing);
      Serial.println();

    lcd.print("Distance: ");
    lcd.print(dist);
    lcd.print("cm  ");
    lcd.setCursor(0,1);
   
    if (distRatio >= 1){
      
      for (int i=1; i<distRatio; i++){
        
        lcd.write(byte(5));
        parts = (distRatio-i)*5;
        //Serial.println(parts);
        //lastParts = i;      
      }
      
      lastDistRatio = distRatio;
    }
    else 
    {
      parts=distRatio*5;
      //lastParts = parts;
      lastDistRatio = distRatio;
    }
    switch (parts){
      
      case 0:
      lcd.write(byte(0));
      lcd.write("               ");
      break;
      
      case 1:
      lcd.write(byte(1));
      lcd.write("               ");
      break;
      
      case 2:
      lcd.write(byte(2));
      lcd.write("               ");
      break;
      
      case 3:
      lcd.write(byte(3));
      lcd.write("               ");
      break;
      
      case 4:
      lcd.write(byte(4));
      lcd.write("               ");
      break;
      
      case 5:
      lcd.write(byte(5));
      lcd.write("               ");
      break;
    }
    lastLCDWrite = lcdWrite;
  }

}
