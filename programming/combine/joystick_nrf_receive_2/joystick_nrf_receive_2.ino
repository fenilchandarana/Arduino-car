#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// NRF24 setup
RF24 radio(16, 17); // CE, CSN pins
const byte address[6] = "00001";

// Motor control pins for L298N
int enA = 25;  // Enable A
int in1 = 26;  // Input 1
int in2 = 27;  // Input 2
int enB = 12;  // Enable B
int in3 = 13;  // Input 3
int in4 = 14;  // Input 4

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Initialize motor pins to a low state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);  // Set PWM speed to zero
  analogWrite(enB, 0);  // Set PWM speed to zero

  Serial.println("Receiver ready and listening...");
}

void loop() {
  if (radio.available()) {
    char buffer[32] = "";
    radio.read(buffer, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination

    Serial.println("Received Message:");
    Serial.println(buffer);

    int pwmSpeed1, pwmSpeed2;
    char direction1[10], direction2[10];

    // Example of expected format: "150,forward,0,right"
    if (sscanf(buffer, "%d,%9[^,],%d,%9s", &pwmSpeed1, direction1, &pwmSpeed2, direction2) == 4) {
      // Determine which direction to apply based on non-zero PWM value
      if (pwmSpeed1 != 0) {
        applyMotorControl(direction1, pwmSpeed1);
      } else if (pwmSpeed2 != 0) {
        applyMotorControl(direction2, pwmSpeed2);
      } else {
        Serial.println("no movement");
        stopMotors();
      }
    } else {
      Serial.println("Error parsing the buffer");
    }

    Serial.println("----- End of Data -----\n");
  }
}

void applyMotorControl(const char* direction, int pwm) {
  analogWrite(enA, pwm);
  analogWrite(enB, pwm); // Assuming both motors should run at the same speed

  if (strcmp(direction, "forward") == 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    Serial.println("forward");
    Serial.println("  digitalWrite(in1, LOW);");
    Serial.println("  digitalWrite(in2, HIGH);");
    Serial.println("  digitalWrite(in3, HIGH);");
    Serial.println("  digitalWrite(in4, LOW);");
  } else if (strcmp(direction, "backward") == 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    Serial.println("backward");
    Serial.println("  digitalWrite(in1, HIGH);");
    Serial.println("  digitalWrite(in2, LOW);");
    Serial.println("  digitalWrite(in3, LOW);");
    Serial.println("  digitalWrite(in4, HIGH);");
  } else if (strcmp(direction, "left") == 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    Serial.println("left");
    Serial.println("  digitalWrite(in1, HIGH);");
    Serial.println("  digitalWrite(in2, LOW);");
    Serial.println("  digitalWrite(in3, HIGH);");
    Serial.println("  digitalWrite(in4, LOW);");
  } else if (strcmp(direction, "right") == 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    Serial.println("right");
    Serial.println("  digitalWrite(in1, LOW);");
    Serial.println("  digitalWrite(in2, HIGH);");
    Serial.println("  digitalWrite(in3, LOW);");
    Serial.println("  digitalWrite(in4, HIGH);");
  }

  Serial.print("enA "); Serial.println(pwm);
  Serial.print("enB "); Serial.println(pwm);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  Serial.println("Motors Stopped");
}
