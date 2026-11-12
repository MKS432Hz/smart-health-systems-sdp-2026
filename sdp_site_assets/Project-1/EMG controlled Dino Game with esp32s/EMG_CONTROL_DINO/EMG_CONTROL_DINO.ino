// ESP32 S Dev Board EMG Dino Game Control

const int emgPin = 34;               // Use GPIO34 (input-only ADC pin)
const int threshold = 300;          // Adjust threshold (0–4095 for ESP32 ADC)
unsigned long lastTrigger = 0;       
const unsigned long cooldown = 300;  // Cooldown in ms

void setup() {
  Serial.begin(9600);
  delay(1000); // Allow serial to stabilize
  pinMode(emgPin, INPUT);
}

void loop() {
  int emgValue = analogRead(emgPin);
  Serial.println(emgValue); 
  if (emgValue < threshold) {
    unsigned long now = millis();
    if (now - lastTrigger > cooldown) {
      Serial.println("JUMP");       // Send jump command to PC
      
      lastTrigger = now;
    }
  }

  delay(10);  // Small delay for stability
}
