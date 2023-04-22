/*
 * Connection
 * HC-05    Arduino
 * TX       2(defined as RX)
 * RX       3(defined as TX)
 * VCC      5V
 * GND      GND
 */

#include <SoftwareSerial.h>

SoftwareSerial BTserial(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:
BTserial.begin(9600);
 pinMode(13, OUTPUT); // put your setup code here, to run once:
 }

void loop() {
  // put your main code here, to run repeatedly:
 if(BTserial.available()>0)
   {     
      char data= BTserial.read(); // reading the data received from the bluetooth module
      switch(data)
      {
        case '1': digitalWrite(13, HIGH);break; // when a is pressed on the app on your smart phone
        case '0': digitalWrite(13, LOW);break; // when d is pressed on the app on your smart phone
        default : break;
      }
   }
   delay(50);
}
