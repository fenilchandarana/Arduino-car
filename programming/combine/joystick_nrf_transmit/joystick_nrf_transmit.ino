#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Joystick connections
const int joystick1X = A0; // Joystick 1 VRX
const int joystick1Y = A1; // Joystick 1 VRY
const int joystick2X = A2; // Joystick 2 VRX
const int joystick2Y = A3; // Joystick 2 VRY

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  pinMode(joystick1X, INPUT);
  pinMode(joystick1Y, INPUT);
  pinMode(joystick2X, INPUT);
  pinMode(joystick2Y, INPUT);
  
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int j1x = analogRead(joystick1X);
  int j1y = analogRead(joystick1Y);
  int j2x = analogRead(joystick2X);
  int j2y = analogRead(joystick2Y);
  
  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d", j1x, j1y, j2x, j2y);
  
  radio.write(&buffer, sizeof(buffer));
  Serial.println(buffer);  // Debugging line to see what is sent
  delay(1000);
}
