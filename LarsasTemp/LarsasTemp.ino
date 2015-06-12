#include <dht.h>

dht DHT;

#define DHT11_PIN A0
const int analogOutPin = 3; // Analog output pin that the LED is attached to
const int relayOutPin = 4;
int pinSwitch = 0;
int stateRelay = 0;
int targetTemp = 50;
int hysteresis = 20;
bool active;


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(57600);
  pinMode(analogOutPin,OUTPUT);
  pinMode(relayOutPin,OUTPUT);
  
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  
}

void loop() {
  DHT11_loop();

  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  
  int LEDtemp = DHT.temperature * 2;
  analogWrite(analogOutPin,LEDtemp);
    
//  for (int i = 0; i <= 15; i++){
//    int x = pow(i, 2); 
//    analogWrite(analogOutPin, x);
//    Serial.print("analogOut = ");Serial.println(x);
//    delay(200);
//  
//  if ( x > (targetTemp + hysteresis) && !active){
//    stateRelay = 1;
//  }
//  else if ( x < (targetTemp - hysteresis) && active){
//    stateRelay = 0;
//    
//  }
//
////   digitalWrite(relayOut, relayState);
//   digitalWrite(relayOutPin, stateRelay);
//  }
//    for (int i = 15; i >= 0; i--){
//    int x = pow(i, 2); 
//    analogWrite(analogOutPin, x);
//    Serial.print("analogOut = ");Serial.println(x);
//    delay(200);
//
//  if ( x > (targetTemp + hysteresis) && !active){
//    stateRelay = 1;
//    active = true;
//  }
//  else if ( x < (targetTemp - hysteresis) && active){
//    stateRelay = 0;
//    active = false;
//  }
//
////   digitalWrite(relayOut, relayState);
//   digitalWrite(relayOutPin, stateRelay);
//  }
  

}
