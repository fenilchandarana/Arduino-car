#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(16, 17); // CE, CSN
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char buffer[50] = "";  // Ensure the buffer size matches the transmitter's buffer size
    radio.read(&buffer, sizeof(buffer));
    Serial.println(buffer);  // Print received data for debugging
    
    int j1x, j1y, j2x, j2y;
    sscanf(buffer, "%d,%d,%d,%d", &j1x, &j1y, &j2x, &j2y);  // Parse the integers from the received string
    
    // Optionally, handle the values here (e.g., to control motors or other outputs)
  }
}
