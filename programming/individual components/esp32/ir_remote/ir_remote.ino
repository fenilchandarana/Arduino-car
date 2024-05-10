#include "IRremote.hpp"

const int irReceiverPin = 22;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(irReceiverPin , ENABLE_LED_FEEDBACK);  
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }
}
