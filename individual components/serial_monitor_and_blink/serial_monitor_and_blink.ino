/*
 * Connection
 * HC-05    Arduino
 * TX       2(defined as RX)
 * RX       3(defined as TX)
 * VCC      5V
 * GND      GND
 */

#include <SoftwareSerial.h>

int RX = 2;
int TX = 3;

SoftwareSerial BTserial(RX, TX);

String BT_data;
String Arduino_data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTserial.begin(9600);
   pinMode(13, OUTPUT); 

}

void loop() {
  // put your main code here, to run repeatedly:

  // app to serial monitor
  if (BTserial.available())
  {
    BT_data = BTserial.readString();
    Serial.println(BT_data);
  }


  // serial monitor to app

  if (Serial.available())
  {
    Arduino_data = Serial.readString();
    BTserial.println(Arduino_data);
  }


  if (BTserial.available())

      {
    char data = BTserial.read(); // reading the data received from the bluetooth module
    switch (data)
    {
      case '1': digitalWrite(13, HIGH); break; // when a is pressed on the app on your smart phone
      case '0': digitalWrite(13, LOW); break; // when d is pressed on the app on your smart phone
      default : break;
    }
  }


}
