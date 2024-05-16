#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Joystick connections
const int joystick1X = A0; // Joystick 1 for forward/backward
const int joystick2Y = A3; // Joystick 2 for left/right

// NRF24L01 setup
RF24 radio(7, 8); // CE, CSN pins
const byte address[6] = "00001";

void setup() {
  pinMode(joystick1X, INPUT);
  pinMode(joystick2Y, INPUT);

  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening(); // Not listening, just sending
}

void loop() {
  int j1x = analogRead(joystick1X);
  int j2y = analogRead(joystick2Y);

  int PWM_j1x, PWM_j2y;
  String directionX, directionY;

  // Map the forward and backward directions
  if (j1x <= 505) {
    PWM_j1x = map(j1x, 0, 505, 255, 0); // Invert mapping for forward
    directionX = "forward";
  } else {
    PWM_j1x = map(j1x, 505, 1023, 0, 255);
    directionX = "backward";
  }

  // Map the left and right directions
  if (j2y <= 507) {
    PWM_j2y = map(j2y, 0, 507, 255, 0); // Invert mapping for right
    directionY = "right";
  } else {
    PWM_j2y = map(j2y, 507, 1023, 0, 255);
    directionY = "left";
  }

  // Format the message as a string for transmission
  char message[32];
  snprintf(message, sizeof(message), "%d,%s,%d,%s", PWM_j1x, directionX.c_str(), PWM_j2y, directionY.c_str());

  // Send the message
  radio.write(message, strlen(message) + 1); // Ensure to include the null terminator for proper string termination at the receiver

  // Print the message to the Serial Monitor
  Serial.print("Sent Message: ");
  Serial.println(message);

  delay(100); // A small delay to prevent overwhelming the receiver
}
