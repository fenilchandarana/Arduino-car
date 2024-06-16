#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// Right motor
int enableRightMotor = 25;
int rightMotorPin1 = 26;
int rightMotorPin2 = 27;

// Left motor
int enableLeftMotor = 12;
int leftMotorPin1 = 13;
int leftMotorPin2 = 14;

#define MAX_MOTOR_SPEED 255 // Adjusted to match PWM range

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) {
  if (rightMotorSpeed < 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else if (rightMotorSpeed > 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else if (leftMotorSpeed > 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));
}

void setUpPinModes() {
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Set up PWM for speed
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel);

  rotateMotor(0, 0);
}

void setup() {
  setUpPinModes();
  Dabble.begin("MyBluetoothCar");
}

void loop() {
  int rightMotorSpeed = 0;
  int leftMotorSpeed = 0;
  Dabble.processInput();

  bool joystickControl = true;

  // Left/Right movement using buttons
  if (GamePad.isSquarePressed()) { // Move Left
    leftMotorSpeed = MAX_MOTOR_SPEED;
    joystickControl = false; // Override joystick
  }

  if (GamePad.isCirclePressed()) { // Move Right
    rightMotorSpeed = -MAX_MOTOR_SPEED;
    joystickControl = false; // Override joystick
  }

  // Joystick for forward/backward movement
  if (joystickControl) {
    int yAxis = GamePad.gety_axis();

    // Map the joystick value to PWM range
    int mappedSpeed = map(abs(yAxis), 0, 6, 0, MAX_MOTOR_SPEED);

    if (yAxis > 0) { // Forward
      rightMotorSpeed = -mappedSpeed;
      leftMotorSpeed = mappedSpeed;
    } else if (yAxis < 0) { // Backward
      rightMotorSpeed = mappedSpeed;
      leftMotorSpeed = -mappedSpeed;
    }
  }

  rotateMotor(rightMotorSpeed, leftMotorSpeed);
}
