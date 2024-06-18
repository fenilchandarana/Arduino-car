#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

// Right motor
int enableRightMotor = 12;
int rightMotorPin1 = 13;
int rightMotorPin2 = 14;

// Left motor
int enableLeftMotor = 25;
int leftMotorPin1 = 26;
int leftMotorPin2 = 27;

#define MAX_MOTOR_SPEED 150 // Adjusted to match PWM range
#define BUTTON_ONLY_SPEED 120
#define BUTTON_WITH_JOYSTICK_SPEED 120

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

  // Joystick for forward/backward movement
  int yAxis = GamePad.gety_axis();
  int mappedSpeed = map(abs(yAxis), 0, 6, 0, MAX_MOTOR_SPEED);

  if (yAxis > 0) { // Forward
    rightMotorSpeed = mappedSpeed;
    leftMotorSpeed = -mappedSpeed;
  } else if (yAxis < 0) { // Backward
    rightMotorSpeed = -mappedSpeed;
    leftMotorSpeed = mappedSpeed;
  }

  // Left/Right movement using buttons
  if (GamePad.isSquarePressed()) { // Move Left
    if (joystickControl && yAxis != 0) {
      rightMotorSpeed = BUTTON_WITH_JOYSTICK_SPEED;
      leftMotorSpeed = 0;
    } else {
      rightMotorSpeed = BUTTON_ONLY_SPEED;
      leftMotorSpeed = BUTTON_ONLY_SPEED;
    }
    joystickControl = false; // Override joystick
  }

  if (GamePad.isCirclePressed()) { // Move Right
    if (joystickControl && yAxis != 0) {
      rightMotorSpeed = 0;
      leftMotorSpeed = -BUTTON_WITH_JOYSTICK_SPEED;
    } else {
      rightMotorSpeed = -BUTTON_ONLY_SPEED;
      leftMotorSpeed = -BUTTON_ONLY_SPEED;
    }
    joystickControl = false; // Override joystick
  }

  rotateMotor(rightMotorSpeed, leftMotorSpeed);
}
