#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Motor pins
int enA = 25;
int in1 = 26;
int in2 = 27;
int enB = 12;
int in3 = 13;
int in4 = 14;

RF24 radio(16, 17); // CE, CSN pins
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  if (radio.available()) {
    char buffer[50] = "";
    radio.read(&buffer, sizeof(buffer));
    Serial.println(buffer);
    
    int j1x, j1y, j2x, j2y;
    sscanf(buffer, "%d,%d,%d,%d", &j1x, &j1y, &j2x, &j2y);

    handleJoystick(j1x, j2y);
  }
}

void handleJoystick(int j1x, int j2y) {
  const int deadZone = 5;  // Dead zone threshold
  const int midJ1 = 505;   // Middle neutral position for J1X
  const int midJ2 = 507;   // Middle neutral position for J2Y
  int speedA, speedB;

  // Dead zone adjustment
  if (abs(j1x - midJ1) <= deadZone) {
    j1x = midJ1;  // Set joystick to middle position if within dead zone
    stopMotors();  // Stop the motors if joystick is near the neutral position
  } else {
    speedA = map(j1x, 0, 1023, 0, 255);  // Map J1X to speedA only if outside dead zone
  }

  if (abs(j2y - midJ2) <= deadZone) {
    j2y = midJ2;  // Set joystick to middle position if within dead zone
    stopMotors();  // Stop the motors if joystick is near the neutral position
  } else {
    speedB = map(j2y, 0, 1023, 0, 255);  // Map J2Y to speedB only if outside dead zone
  }

  // Direction control based on joystick position
  if (j1x < midJ1) { // Forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, speedA);
    analogWrite(enB, speedA);
  } else if (j1x > midJ1) { // Backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, speedA);
    analogWrite(enB, speedA);
  }

  if (j2y > midJ2) { // Left
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, speedB);
    analogWrite(enB, speedB);
  } else if (j2y < midJ2) { // Right
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, speedB);
    analogWrite(enB, speedB);
  }
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}
