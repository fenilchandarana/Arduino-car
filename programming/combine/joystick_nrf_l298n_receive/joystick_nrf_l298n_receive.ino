#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino.h>

// Motor pins
const int enA = 25;
const int in1 = 26;
const int in2 = 27;
const int enB = 12;
const int in3 = 13;
const int in4 = 14;

// NRF setup
RF24 radio(16, 17); // CE, CSN
const byte address[6] = "00001";

// Initialize motor control pins
void setupMotorPins() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

// Initialize all systems
void setup() {
  Serial.begin(9600);
  setupMotorPins();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

// Map the joystick values to PWM values
int mapJoystickToPWM(int value, int center) {
  if (abs(value - center) <= 10) return 0; // Dead zone
  return map(abs(value - center), 0, 518, 0, 255);
}

// Execute motor control based on joystick input
void controlMotors(int j1x, int j2y) {
  // Dead zone check
  if (abs(j1x - 505) > 10) {
    if (j1x < 495) {
      // Move forward
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    } else if (j1x > 515) {
      // Move backward
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    analogWrite(enA, mapJoystickToPWM(j1x, 505));
    analogWrite(enB, mapJoystickToPWM(j1x, 505));
  } else if (abs(j2y - 507) > 10) {
    if (j2y < 497) {
      // Move right
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    } else if (j2y > 517) {
      // Move left
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
    analogWrite(enA, mapJoystickToPWM(j2y, 507));
    analogWrite(enB, mapJoystickToPWM(j2y, 507));
  } else {
    // Stop motors if within dead zone
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }
}

// Main loop
void loop() {
  if (radio.available()) {
    char buffer[50] = "";  // Buffer for receiving data
    radio.read(&buffer, sizeof(buffer));
    int j1x, j1y, j2x, j2y;
    sscanf(buffer, "%d,%d,%d,%d", &j1x, &j1y, &j2x, &j2y);
    controlMotors(j1x, j2y);
  }
}
