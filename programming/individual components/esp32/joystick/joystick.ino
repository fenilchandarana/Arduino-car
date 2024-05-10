// Define the analog pins for the joysticks
const int joystick1X = A0;  // Joystick 1 VRX
const int joystick1Y = A1;  // Joystick 1 VRY
const int joystick2X = A2;  // Joystick 2 VRX
const int joystick2Y = A3;  // Joystick 2 VRY

void setup() {
  // Initialize serial communication at 9600 bps
  Serial.begin(9600);
}

void loop() {
  // Read the analog values from joystick 1
  int j1x = analogRead(joystick1X);
  int j1y = analogRead(joystick1Y);
  
  // Read the analog values from joystick 2
  int j2x = analogRead(joystick2X);
  int j2y = analogRead(joystick2Y);

  // Print the joystick values in the specified format
  Serial.print("J1X=");
  Serial.print(j1x);
  Serial.print(" J1Y=");
  Serial.print(j1y);
  Serial.print(" J2X=");
  Serial.print(j2x);
  Serial.print(" J2Y=");
  Serial.println(j2y);

  // Delay for a short period before repeating
  delay(100);
}
