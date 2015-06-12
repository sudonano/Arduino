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

int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

long pulseCount = 0;
unsigned long pulseTime,lastTime;
double power, elapsedkWh;
int ppwh = 1;
static unsigned long lastShift = 0;
void setup()
{ 

  lcd.begin (16,2); //  <<----- My LCD was 16x2
  Serial.begin(9600);
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
  //lcd.print("WattHour Meter");  
  
  //SETUP INTERRUPT
  attachInterrupt(0, onPulse, FALLING);
}

void loop()
{
for (int i = 0; i < 15; i++)
  {  
  unsigned long shiftTime = millis();
  if (shiftTime - lastShift > 500)
  {
  n++;
  
  lastShift = shiftTime;
  lcd.clear();
  lcd.setCursor(n,0);
  lcd.print("Power: ");
  lcd.print(power);
  lcd.setCursor(0,1);
  lcd.print("Used kWh: ");
  lcd.print(elapsedkWh);
  if (n >= 15)
  n = -4;
  }
  }
////BUTTONDISPLAY
// if (elapsedkWh == 0)
//   {
//     lcd.setCursor(11,1); 
//     lcd.print("000");
//   }
// else if (elapsedkWh < 10)
//   {
//     lcd.setCursor(11,1); 
//     lcd.print("00");
//     lcd.setCursor(15,1); 
//     lcd.print(elapsedkWh,DEC);
//   } 
// else if (elapsedkWh < 100)
//   {
//     lcd.setCursor(13,1);
//     lcd.print("0");
//     lcd.setCursor(14,1); 
//     lcd.print(elapsedkWh,DEC);
//   } 
// else if (elapsedkWh < 1000)
//   {
//     lcd.setCursor(13,1);
//     lcd.print(elapsedkWh,DEC);
//   }
}

void onPulse()
{
  static unsigned long last_interr = 0;
  unsigned long interr_time = millis();
  if (interr_time - last_interr > 100)
  {
  last_interr = interr_time;
  lastTime = pulseTime;
  pulseTime = micros();
  pulseCount++;
  
  power = (3600000000.0 / (pulseTime - lastTime))/ppwh;
  
  elapsedkWh = (1.0*pulseCount/(ppwh*1000));
  
  //Print the values.
  Serial.print(power,4);
  Serial.print(" ");
  Serial.println(elapsedkWh,3);
  }

}
