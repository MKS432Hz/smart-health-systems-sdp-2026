void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud
}

void loop() {
  for (int pin = A0; pin <= A7; pin++) {
    int value = analogRead(pin); // Read value from analog pin
    Serial.print(value);         // Send the value
    if (pin < A7) Serial.print(","); // Separate values with a comma
  }
  Serial.println(); // End line after all pins
  delay(1);        // Adjust based on sampling rate requirements
}
