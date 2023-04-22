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

}

void loop() {
  // put your main code here, to run repeatedly:

    // app to serial monitor
if (BTserial.available())
{
  BT_data = BTserial.readString();
  Serial.println(BT_data);
  delay(200);
}

  // serial monitor to app
  if (Serial.available())
  {
    Arduino_data = Serial.readString();
    BTserial.println(Arduino_data);
    delay(200);
  }
}
