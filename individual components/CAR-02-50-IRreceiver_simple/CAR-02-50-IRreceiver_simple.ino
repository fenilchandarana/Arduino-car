/*  ASBSGSR_-_2800_-_Infrared_Remote_control_demo_simplified
 * 
 * This sketch Demonstrates receiving NEC IR codes with IRrecv.
 * 
 * 
 * Components
 * ----------
 *  - Arduino Uno
 *  - an Infrared (IR) receiver and transmitter that are 
 *  - compatible with the Arduino-IRremote library.
 *  - Jumper wires
 *  - Mini breadboard
 *  
 *  Libraries
 *  ---------
 *  Arduino-IRremote: https://github.com/Arduino-IRremote/Arduino-IRremote
 *
 * Connections
 * -----------
 *  
 * For the IR receiver: 
 * 
 * IR Receiver   |    Arduino
 * --------------------------
 *       V+      |      5V
 *       GND     |      GND
 *       OUT     |      2
 * 
 *     
 *  
 *  Created on January 12 2023 by Peter Dalmaris
 * 
 */

#include <IRremote.h>

#define IR_RECEIVE_PIN 2

void setup() {
  Serial.begin(115200);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // We have an unknown protocol here, print more info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
    }
    Serial.println();
    IrReceiver.resume();
  }
}