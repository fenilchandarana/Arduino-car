/*  0240 - simple DC motor control with the L298N controller IC
 * 
 * This sketch shows you how to control tho small 5V DC motors using the L298N controller
 * breakout.
 * 
 * The sketch speeds up the motors in three steps towards one direction,
 * and then does the same in the other direction.
 * 
 * 
 * Components
 * ----------
 *  - Arduino Uno
 *  - a L298N motor controller breakout
 *  - two small 5V DC motors (you can play with only one motor by commenting out the instructions for motor 2)
 *  
 *  Libraries
 *  ---------
 *  None
 *
 * Connections
 * -----------
 *  
 * Arduino   |   L298N 
 * ---------------------
 *   GND     |   GND
 *   5       |   IN1
 *   4       |   IN2
 *           |   OUT1  --> Motor 1 wire 1
 *           |   OUT2  --> Motor 1 wire 2
 *           |   +12V  --> 5V power supply 
 *           |   GND   --> GND power supply
 *     
 * More information
 * ----------------
 * L298N datasheet: http://www.ti.com/lit/ds/symlink/l293.pdf
 *  
 *  Created on January 12 2022 by Peter Dalmaris
 * 
 */

int speed1 = 5;      //Controls speed (PWM) for motor 1
int direction1 = 4;  //Controls direction for motor 1

bool direction = HIGH;  // Start the motor by moving it towards one direction. Whether it is
                        // clockwise or anticlockwise depends on how you have connected the
                        // motor's coil.

void setup() {
  pinMode(direction1, OUTPUT);
  pinMode(speed1, OUTPUT);

  digitalWrite(speed1, LOW);
  digitalWrite(direction1, LOW);
  delay(1000);
}

void loop() {
  digitalWrite(direction1, direction);  // Direction control motor 1
  
  analogWrite(speed1, 150);             // PWM Speed Control motor 1
  delay(5000);
//  analogWrite(speed1, 100);  // PWM Speed Control motor 1
//  delay(5000);
//  analogWrite(speed1, 200);  // PWM Speed Control motor 1
//  delay(5000);

//  direction = !direction;  // Switch direction
//  analogWrite(speed1, 0);  // Stop motor 1 before starting again in the opposite direction
//  delay(8000);
}
