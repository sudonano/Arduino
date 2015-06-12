//https://www.electronicsblog.net/
//Arduino LCD horizontal progress bar using custom characters
//DEFINE AND DECLARE FOR DISPLAY
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

#define lenght 16.0

double percent=100.0;
unsigned char b;
unsigned int peace;

// custom charaters

LiquidCrystal.lcd(12, 11, 5, 4, 3, 2);
byte p1[8] = {
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10};

byte p2[8] = {
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18};

byte p3[8] = {
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C};

byte p4[8] = {
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E};

byte p5[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F};

void setup()   {

  delay(100);
  lcd.createChar(0, p1);
  lcd.createChar(1, p2);
  lcd.createChar(2, p3);
  lcd.createChar(3, p4);
  lcd.createChar(4, p5);

  lcd.begin(16, 2);

  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
}

void loop()
{

  lcd.setCursor(0, 0);

  //ADC conversion

  unsigned int value = analogRead(0);

  percent = value/1024.0*100.0;

  lcd.print(value);

  lcd.print(" - ");

  lcd.print(percent);
  lcd.print(" %   ");

  lcd.setCursor(0,1);

  double a=lenght/100*percent;

 // drawing black rectangles on LCD

  if (a>=1) {

    for (int i=1;i<a;i++) {

      lcd.write(4);

      b=i;
    }

    a=a-b;

  }

  peace=a*5;

// drawing charater's colums

  switch (peace) {

  case 0:

    break;

  case 1:
    lcd.write(0);

    break;

  case 2:
    lcd.write(1);
    break;

  case 3:
    lcd.write(2);
    break;

  case 4:
    lcd.write(3);
    break;

  }

//clearing line
  for (int i =0;i<(lenght-b);i++) {

    lcd.print(" ");
  }

  ;
}

