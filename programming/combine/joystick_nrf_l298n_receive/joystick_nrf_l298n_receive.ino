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

void setupMotorPins() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  setupMotorPins();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

// Function to map joystick values to PWM output with a dead zone
int mapJoystickToPWM(int value, int center) {
  if (abs(value - center) <= 10) return 0; // Dead zone
  return map(abs(value - center), 0, 518, 0, 255);
}

// Function to control motors based on joystick inputs
void controlMotors(int j1x, int j2y) {
  int baseSpeedA = mapJoystickToPWM(j1x, 505);
  int baseSpeedB = mapJoystickToPWM(j2y, 507);

  if (abs(j1x - 505) > 10) {
    digitalWrite(in1, j1x < 495 ? LOW : HIGH);
    digitalWrite(in2, j1x < 495 ? HIGH : LOW);
    digitalWrite(in3, j1x < 495 ? HIGH : LOW);
    digitalWrite(in4, j1x < 495 ? LOW : HIGH);
    analogWrite(enA, baseSpeedA);
    analogWrite(enB, baseSpeedA);
  } else if (abs(j2y - 507) > 10) {
    digitalWrite(in1, j2y > 517 ? HIGH : LOW);
    digitalWrite(in2, j2y > 517 ? LOW : HIGH);
    digitalWrite(in3, j2y > 517 ? HIGH : LOW);
    digitalWrite(in4, j2y > 517 ? LOW : HIGH);
    if (j2y > 517) {
      analogWrite(enA, baseSpeedB / 2);
      analogWrite(enB, baseSpeedB);
    } else {
      analogWrite(enA, baseSpeedB);
      analogWrite(enB, baseSpeedB / 2);
    }
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }
}

void loop() {
  if (radio.available()) {
    char buffer[50] = "";
    radio.read(&buffer, sizeof(buffer));
    int j1x, j1y, j2x, j2y;
    sscanf(buffer, "%d,%d,%d,%d", &j1x, &j1y, &j2x, &j2y);
    controlMotors(j1x, j2y);
  }
}
