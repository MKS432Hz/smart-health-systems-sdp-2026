void setup() {
  // Initialize the serial communication
  Serial.begin(9600); // Set baud rate to 9600
  while (!Serial) {
    // Wait for the serial port to connect (for some boards like Leonardo, not necessary for Nano)
  }

  // Configure A0 as an input
  pinMode(A0, INPUT);
}

void loop() {
  // Read the analog signal from pin A0
  int sensorValue = analogRead(A0);

  // Display the value on the Serial Monitor
  Serial.print("Analog Value: ");
  Serial.println(sensorValue);

  // Add a short delay for stability
  delay(1); // Adjust delay as needed (100ms here)
}
