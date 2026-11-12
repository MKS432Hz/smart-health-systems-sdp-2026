#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Wi-Fi credentials..
//#define WIFI_SSID "Shobhit_University_BT1"
//#define WIFI_PASSWORD "0000000000@123"
//
#define WIFI_SSID "iQOO z3 MKS"
#define WIFI_PASSWORD "iqooz3mks"

// Firebase credentials
//#define API_KEY "AIzaSyD6OBjBl6GB7iTMjNhgL25GkCC7NtdF8iU"
//#define DATABASE_URL "https://realtimeeegdatafb-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define API_KEY "AIzaSyCOHTeE0AAPR3luwWv-optbK5YSlTANLBs"
#define DATABASE_URL "https://realtimeeegviewer-default-rtdb.firebaseio.com/"


// Firebase email/password authentication
#define USER_EMAIL "manoj.sharma@shobhituniversity.ac.in"
#define USER_PASSWORD "Sumsum@147"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// EEG pin and sampling
const int EEG_PIN = 34; // GPIO34: analog input
#define SAMPLE_INTERVAL_US (1000000 / 256)  // 256 Hz

unsigned long previousMicros = 0;

// ------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Wi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Set email and password
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Connecting to Firebase...");
  while (!Firebase.ready()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Firebase connected & authenticated.");
}

// ------------------------------------------------------------------------
void loop() {
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros >= SAMPLE_INTERVAL_US) {
    previousMicros = currentMicros;

    // Read EEG signal
    float raw = analogRead(EEG_PIN);
    float filtered = EEGFilter(raw);

   // Serial.print("Filtered EEG: ");
    Serial.println(filtered);

    // Send to Firebase
    if (Firebase.RTDB.setFloat(&fbdo, "/EEG/filtered", filtered)) {
      Serial.println("✅ Data sent to Firebase");
    } else {
      Serial.print("❌ Firebase error: ");
      Serial.println(fbdo.errorReason());
    }
  }
}

// ------------------------------------------------------------------------
// EEG Band-Pass Filter (4-stage example)
float EEGFilter(float input) {
  float output = input;

  // Stage 1
  static float z1_1, z2_1;
  float x1 = output - -0.95391350 * z1_1 - 0.25311356 * z2_1;
  output = 0.00735282 * x1 + 0.01470564 * z1_1 + 0.00735282 * z2_1;
  z2_1 = z1_1;
  z1_1 = x1;

  // Stage 2
  static float z1_2, z2_2;
  float x2 = output - -1.20596630 * z1_2 - 0.60558332 * z2_2;
  output = 1.00000000 * x2 + 2.00000000 * z1_2 + 1.00000000 * z2_2;
  z2_2 = z1_2;
  z1_2 = x2;

  // Stage 3
  static float z1_3, z2_3;
  float x3 = output - -1.97690645 * z1_3 - 0.97706395 * z2_3;
  output = 1.00000000 * x3 + -2.00000000 * z1_3 + 1.00000000 * z2_3;
  z2_3 = z1_3;
  z1_3 = x3;

  // Stage 4
  static float z1_4, z2_4;
  float x4 = output - -1.99071687 * z1_4 - 0.99086813 * z2_4;
  output = 1.00000000 * x4 + -2.00000000 * z1_4 + 1.00000000 * z2_4;
  z2_4 = z1_4;
  z1_4 = x4;

  return output;
}
