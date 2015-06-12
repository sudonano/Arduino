//
//int x = 0;
//
////DEFINE AND DECLARE FOR DISPLAY
//#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>
//
//#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
//#define BACKLIGHT_PIN     3
//#define En_pin  2
//#define Rw_pin  1
//#define Rs_pin  0
//#define D4_pin  4
//#define D5_pin  5
//#define D6_pin  6
//#define D7_pin  7
//
//int n = 1;
//
//LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//
////END DECLARE VARIABLES AND LIBS
//
//void setup() {
//
//  //SETUP DISPLAY
//    lcd.begin (16,2); //  <<----- My LCD was 16x2
//
// 
//  // Switch on the backlight
//  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
//  lcd.setBacklight(HIGH);
//  lcd.home (); // go home
//
//  lcd.print("WattHour Meter");  
//  //END SETUP DISPLAY
//  
//  //SETUP INTERRUPT
//  attachInterrupt(0, pulse, FALLING);
//  //END SETUP INTERRUPT
//}
//
//void loop() {
//
//  
////BUTTONDISPLAY
// if (x == 0)
//   {
//     lcd.setCursor(13,1); 
//     lcd.print("000");
//   }
// else if (x < 10)
//   {
//     lcd.setCursor(13,1); 
//     lcd.print("00");
//     lcd.setCursor(15,1); 
//     lcd.print(x,DEC);
//   } 
// else if (x < 100)
//   {
//     lcd.setCursor(13,1);
//     lcd.print("0");
//     lcd.setCursor(14,1); 
//     lcd.print(x,DEC);
//   } 
// else if (x < 1000)
//   {
//     lcd.setCursor(13,1);
//     lcd.print(x,DEC);
//   }
// else if (x >= 1000)
//   { 
//     x = 0;
//   }
//}  
//   
//void pulse()
//{
//  static unsigned long last_interr = 0;
//  unsigned long interr_time = millis();
//  if (interr_time - last_interr > 100)
//  {
//  x++;
//  }
//  last_interr = interr_time;
//}
