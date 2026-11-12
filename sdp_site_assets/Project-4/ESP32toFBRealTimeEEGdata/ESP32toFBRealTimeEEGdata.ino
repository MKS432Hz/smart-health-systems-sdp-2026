#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Replace with your network credentials
#define WIFI_SSID "Shobhit_University_BT1"
#define WIFI_PASSWORD "0000000000@123"

// Firebase project credentials
#define API_KEY "AIzaSyD6OBjBl6GB7iTMjNhgL25GkCC7NtdF8iU"
#define DATABASE_URL "https://realtimeeegdatafb-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase and auth objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// For EEG analog input
const int EEG_PIN = 34;  // Use GPIO34 for analog input (adjust based on wiring)

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Anonymous sign-in
  auth.user.email = "";
  auth.user.password = "";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Wait for token generation
  while (auth.token.uid == "") {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Firebase ready");
}

void loop() {
  unsigned long currentMillis = millis();

  // Send data every 100 ms (10 Hz sampling rate; adjust as needed)
  if (currentMillis - sendDataPrevMillis > 100) {
    sendDataPrevMillis = currentMillis;

    int eegRaw = analogRead(EEG_PIN);  // Read EEG signal

    Serial.print("EEG Raw: ");
    Serial.println(eegRaw);

    // Send to Firebase (under "/EEG/raw" path)
    if (Firebase.RTDB.setInt(&fbdo, "/EEG/raw", eegRaw)) {
      Serial.println("Data sent to Firebase");
    } else {
      Serial.print("Firebase Error: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
