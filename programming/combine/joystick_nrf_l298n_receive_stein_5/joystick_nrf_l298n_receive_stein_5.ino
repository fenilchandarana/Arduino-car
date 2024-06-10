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

struct Motor {
  enum Direction {OFF, FORWARD, BACKWARD};
  Motor(int pwm_pin, int fwd_pin, int bwd_pin)
    : pwm(pwm_pin), fwd(fwd_pin), bwd(bwd_pin) {}
  void setDirection(Direction dir) const {
    switch (dir) {
    case FORWARD:
      digitalWrite(fwd, HIGH);
      digitalWrite(bwd, LOW);
      break;
    case BACKWARD:
      digitalWrite(bwd, HIGH);
      digitalWrite(fwd, LOW);
      break;
    case OFF:
    default:
      digitalWrite(fwd, LOW);
      digitalWrite(bwd, LOW);
      break;
    }
  }
  void setSpeed(int speed) const {
    analogWrite(pwm, speed);
  }
  void begin() const {
    pinMode(pwm, OUTPUT);
    pinMode(fwd, OUTPUT);
    pinMode(bwd, OUTPUT);
    setSpeed(0);
    setDirection(OFF);
  }
  const int pwm;
  const int fwd;
  const int bwd;
};

const Motor left_motor(enA, in2, in1);
const Motor right_motor(enB, in3, in4);

// NRF setup
RF24 radio(16, 17); // CE, CSN
const byte address[6] = "00001";


void setup() {
  //Serial.begin(9600);
  left_motor.begin();
  right_motor.begin();

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}


int mapValue(int value) {
  static const int center = 507;
  static const int dead = 10;
  if (value > center + dead) // backward
    return -map(value, center + dead, 1023, 0, 255);
  else if (value < center - dead) // forward
    return map(value, center - dead, 0, 0, 255);
  return 0;
}

Motor::Direction calcDirection(int speed) {
  return speed == 0 ? Motor::OFF :
    speed > 0 ? Motor::FORWARD :
    Motor::BACKWARD;
}


void controlMotors(int left_joy, int right_joy) {
  int left_speed = mapValue(left_joy);
  int right_speed = mapValue(right_joy);
  left_motor.setDirection(calcDirection(left_speed));
  right_motor.setDirection(calcDirection(right_speed));
  left_motor.setSpeed(abs(left_speed));
  right_motor.setSpeed(abs(right_speed));
}

void loop() {
  if (radio.available()) {
    char buffer[50] = "";
    radio.read(&buffer, sizeof(buffer));
    int j1x, j1y, j2x, j2y;
    sscanf(buffer, "%d,%d,%d,%d", &j1x, &j1y, &j2x, &j2y);
    controlMotors(j1x, j2x);
  }
}
