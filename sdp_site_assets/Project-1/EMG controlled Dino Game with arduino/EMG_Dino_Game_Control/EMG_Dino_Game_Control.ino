// Arduino Nano EMG Dino Game Control

const int emgPin = A1;             // Analog input pin A0

const int threshold = 400;         // Adjust based on your EMG signal (0–1023)
unsigned long lastTrigger = 0;     // Last time jump was sent
const unsigned long cooldown = 300; // Cooldown (ms) to avoid rapid jumps

void setup() {
  Serial.begin(9600);
  pinMode(emgPin, INPUT);
}

void loop() {
  int emgValue = analogRead(emgPin);

  if (emgValue > threshold) {
    unsigned long now = millis();
    if (now - lastTrigger > cooldown) {
      Serial.println("JUMP");      // Send jump command to laptop
      Serial.println(emgValue);  
      lastTrigger = now;
    }
  }

  delay(10);  // Small delay to stabilize reading
}
