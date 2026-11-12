#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define LED_PIN 2  // Change to 2 if you're using the built-in LED on NodeMCU-32S

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_LED_Control");  // Bluetooth device name
  Serial.println("Bluetooth device is ready to pair");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED OFF
}

void loop() {
  if (SerialBT.available()) {
    String incoming = SerialBT.readStringUntil('\n');
    incoming.trim();  // Remove any whitespace or newlines
    Serial.print("Received: ");
    Serial.println(incoming);

    if (incoming == "1") {
      digitalWrite(LED_PIN, HIGH);
      SerialBT.println("LED is ON");
      Serial.println("LED turned ON");
    } else if (incoming == "0") {
      digitalWrite(LED_PIN, LOW);
      SerialBT.println("LED is OFF");
      Serial.println("LED turned OFF");
    } else {
      SerialBT.println("Unknown command: " + incoming);
      Serial.println("Unknown command received");
    }
  }
}
